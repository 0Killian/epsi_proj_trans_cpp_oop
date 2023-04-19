//
// Created by Killian on 14/03/2023.
//
#include <engine/Application.h>
#include <engine/RandomNumberGenerator.h>
#include <MainMenuScene.h>
#include "engine/renderers/RendererObjects.h"

std::unique_ptr<Application> Application::s_instance;

Application::Application()
{
    // Initialize the subsystems
    RandomNumberGenerator::Init();

    CreateWindow(sf::VideoMode({DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT}), false);

#if defined(PLATFORM_WINDOWS)
    //m_contextId = wglGetCurrentContext();
#elif defined(PLATFORM_LINUX)
    //m_contextId = glXGetCurrentContext();
#elif defined(PLATFORM_MACOS)
#error "MacOS is not supported yet"
#endif

    _m_window = std::make_unique<Engine::Window>("Test", Engine::Vector2<uint32_t>{ DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT });
}

void Application::RunMainLoop()
{
    try
    {
        // Initialize the renderer
        m_renderer = _m_window->CreateRenderer(Engine::RendererAPI::RendererAPI_OpenGL);

        auto vertexShader = Engine::CreateShader(m_renderer, Engine::ShaderType::ShaderType_Vertex);
        vertexShader->LoadFromFile("assets/shaders/vertexShader.vert.spv");

        auto fragmentShader = Engine::CreateShader(m_renderer, Engine::ShaderType::ShaderType_Fragment);
        fragmentShader->LoadFromFile("assets/shaders/fragmentShader.frag.spv");

#pragma pack(push, 1)
        struct Vertex
        {
            float x = 0;
            float y = 0;
            float z = 0;

            Vertex() = default;
            Vertex(float x, float y, float z) : x(x), y(y), z(z) {}

            [[nodiscard]] static Engine::VertexBufferLayout GetLayout()
            {
                return {
                    { "aPos", Engine::ShaderBaseType::ShaderBaseType_Float3 }
                };
            }
        };
#pragma pack(pop)

        std::vector<Vertex> vertices = {
            { -0.5f, -0.5f, 0.0f },
            {  0.5f, -0.5f, 0.0f },
            { -0.5f,  0.5f, 0.0f },
            {  0.5f,  0.5f, 0.0f }
        };

        auto vertexBuffer = Engine::CreateVertexBuffer<Vertex>(m_renderer, Engine::VertexBufferUsage::VertexBufferUsage_Static);
        vertexBuffer->SetData(vertices.data(), vertices.size());

        std::vector<uint32_t> indices = {
            0, 1, 2,
            1, 2, 3
        };

        auto indexBuffer = Engine::CreateIndexBuffer(m_renderer, Engine::IndexBufferUsage::IndexBufferUsage_Static);
        indexBuffer->SetData(indices.data(), indices.size());

        auto pipeline = CreatePipeline<Vertex>(
                m_renderer,
                { 0, 0 }, { DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT },
                { 0, 0 }, { DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT },
                Engine::PrimitiveTopology::Triangles);
        pipeline->SetVertexShader(vertexShader);
        pipeline->SetFragmentShader(fragmentShader);
        pipeline->SetVertexBuffer(vertexBuffer);
        pipeline->SetIndexBuffer(indexBuffer);

        pipeline->Bind();

        // Initialize the thread pool
        m_threadPool.Init();

        // Restart the clocks so that the first frame's delta time
        // is the lowest possible
        m_clock.restart();

        m_currentScene = std::make_unique<MainMenuScene>();
        m_currentScene->Init();

        // Main loop
        while(m_running)
        {
            // Timing and FPS calculations
            float delta_time = m_clock.restart().asSeconds();
            m_timer += delta_time;

            if(m_timer >= 1.0f)
            {
                m_timer = 0.0f;
                m_window.setTitle(std::string(WINDOW_TITLE) + " - " + std::to_string(frames) + " FPS");
                _m_window->SetTitle(std::string(WINDOW_TITLE) + " - " + std::to_string(frames) + " FPS");
                frames = 0;
            }

            m_renderer->Clear(Engine::Color::Green());
            pipeline->Render();
            m_renderer->SwapBuffers();

            // Update and render the frame
            //Update(delta_time);
            //Render();

            frames++;
        }
    }
    catch(std::exception& e)
    {
        SPDLOG_ERROR("Exception caught: {}", e.what());
    }

    // Destroy the current scene before terminating the thread pool to let it
    // finish its tasks
    m_currentScene.reset();

    // Terminate the thread pool
    m_threadPool.Terminate();

    m_threadPool.RethrowExceptions();
}

void Application::Update(float deltaTime)
{
    m_threadPool.RethrowExceptions();

    // Event pump
    sf::Event event{};
    while(m_window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed
        || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            m_running = false;
            return;
        }
        else if(event.type == sf::Event::Resized)
        {
            m_windowWidth = event.size.width;
            m_windowHeight = event.size.height;
        }
        else if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::F11 || (event.key.code == sf::Keyboard::Enter && event.key.alt))
            {
                if(m_fullscreen)
                {
                    CreateWindow(sf::VideoMode({DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT}), false);
                }
                else if(!sf::VideoMode::getFullscreenModes().empty())
                {
                    sf::VideoMode mode = sf::VideoMode::getFullscreenModes()[0];
                    CreateWindow(mode, true);
                }

                sf::Event resizeEvent{};
                resizeEvent.type = sf::Event::Resized;
                resizeEvent.size.width = m_windowWidth;
                resizeEvent.size.height = m_windowHeight;

                m_currentScene->HandleEvent(resizeEvent);

                continue;
            }
        }

        m_currentScene->HandleEvent(event);
    }

    // In case the window was closed without an event notifying it
    if(!m_window.isOpen())
    {
        throw std::runtime_error("[Application] Window closed unexpectedly");
    }

    // Update the current scene
    m_currentScene->Update(deltaTime);
}

void Application::Render()
{
    // Clear the window
    m_window.clear();

    // Render the current scene
    m_currentScene->Render(m_window);

    // Display the frame to the screen
    m_window.display();
}

void Application::CreateWindow(sf::VideoMode mode, bool fullscreen)
{
    m_window.create(
            mode,
            WINDOW_TITLE.data(), sf::Style::Default | (fullscreen ? sf::Style::Fullscreen : 0));
    m_window.setVerticalSyncEnabled(true);
    m_window.setKeyRepeatEnabled(false);

    //std::unique_lock<std::mutex> lock(m_threadsContextMutex);
    //for(auto& context : m_threadsContext)
    //{
#if defined(PLATFORM_WINDOWS)
        //wglShareLists(m_contextId, context);
#elif defined(PLATFORM_LINUX)
        // TODO
#endif
    //}
    //lock.unlock();

    m_windowWidth = mode.size.x;
    m_windowHeight = mode.size.y;
    m_fullscreen = fullscreen;
}

#if defined(PLATFORM_WINDOWS)
//void Application::AddContext(HGLRC context)
#elif defined(PLATFORM_LINUX)
//void Application::AddContext(GLXContext context)
#elif defined(PLATFORM_MACOS)
#error "Not implemented"
#endif
//{
    //std::unique_lock<std::mutex> lock(m_threadsContextMutex);
    //m_threadsContext.push_back(context);
//}

void Application::Exit()
{
    s_instance.reset();
}