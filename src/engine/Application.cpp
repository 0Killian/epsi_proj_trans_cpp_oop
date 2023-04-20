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

        // vertices of a cube
        // 8 vertices
        std::vector<Engine::Pipeline3DTextured::Vertex> vertices = {
            { { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f } },
            { {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f } },
            { { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f } },
            { {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f } },
            { { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f } },
            { {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f } },
            { { -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f } },
            { {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f } },
        };

        auto vertexBuffer = Engine::CreateVertexBuffer<Engine::Pipeline3DTextured::Vertex>(
            m_renderer,
            Engine::VertexBufferUsage::VertexBufferUsage_Static
        );

        vertexBuffer->SetData(vertices.data(), vertices.size());

        std::vector<uint32_t> indices = {
            0, 1, 2, 2, 1, 3,
            1, 5, 3, 3, 5, 7,
            5, 4, 7, 7, 4, 6,
            4, 0, 6, 6, 0, 2,
            2, 3, 6, 6, 3, 7,
            4, 5, 0, 0, 5, 1
        };

        auto indexBuffer = Engine::CreateIndexBuffer(m_renderer, Engine::IndexBufferUsage::IndexBufferUsage_Static);
        indexBuffer->SetData(indices.data(), indices.size());

        auto pipeline = CreatePipeline3DTextured(
            m_renderer,
            { 0, 0 }, { DEFAULT_WINDOW_WIDTH / 2, DEFAULT_WINDOW_HEIGHT },
            { 0, 0 }, { DEFAULT_WINDOW_WIDTH / 2, DEFAULT_WINDOW_HEIGHT },
            Engine::PrimitiveTopology::Triangles);
        pipeline->SetVertexBuffer(vertexBuffer);
        pipeline->SetIndexBuffer(indexBuffer);

        std::shared_ptr<Engine::Texture> texture = Engine::CreateTexture(m_renderer);
        texture->LoadFromFile("assets/textures/main_menu.png");
        pipeline->SetTexture(texture);

        Engine::Vector3<float> objectPosition = {0, 0.0f, 2.0f};

        Engine::Vector3<float> cameraPosition = { 0.0f, 0.0f, -5.0f };
        Engine::Vector3<float> cameraTarget = objectPosition;

        Engine::Matrix4<float> model = Engine::Matrix4<float>::Translation(objectPosition) * Engine::Matrix4<float>::Rotation({0, 0, 45.0f});
        Engine::Matrix4<float> view = Engine::Matrix4<float>::LookAt(cameraPosition, cameraTarget, { 0.0f, 1.0f, 0.0f });
        Engine::Matrix4<float> projection = Engine::Matrix4<float>::PerspectiveProjection(
                45.0f,
                static_cast<float>(DEFAULT_WINDOW_WIDTH / 2.0f) / static_cast<float>(DEFAULT_WINDOW_HEIGHT),
                0.1f,
                1000.0f);

        pipeline->SetModelMatrix(model);
        pipeline->SetViewMatrix(view);
        pipeline->SetProjectionMatrix(projection);

        std::vector<Engine::Pipeline3D::Vertex> vertices2 = {
            { { -0.5f, -0.5f, -0.5f }, Engine::Color::Red() },
            { {  0.5f, -0.5f, -0.5f }, Engine::Color::Green() },
            { { -0.5f,  0.5f, -0.5f }, Engine::Color::Blue() },
            { {  0.5f,  0.5f, -0.5f }, Engine::Color::Yellow() },
            { { -0.5f, -0.5f,  0.5f }, Engine::Color::Red() },
            { {  0.5f, -0.5f,  0.5f }, Engine::Color::Green() },
            { { -0.5f,  0.5f,  0.5f }, Engine::Color::Blue() },
            { {  0.5f,  0.5f,  0.5f }, Engine::Color::Yellow() },
        };

        auto vertexBuffer2 = Engine::CreateVertexBuffer<Engine::Pipeline3D::Vertex>(
            m_renderer,
            Engine::VertexBufferUsage::VertexBufferUsage_Static
        );
        vertexBuffer2->SetData(vertices2.data(), vertices2.size());

        auto pipeline2 = CreatePipeline3D(
            m_renderer,
            { DEFAULT_WINDOW_WIDTH / 2, 0 }, { DEFAULT_WINDOW_WIDTH / 2, DEFAULT_WINDOW_HEIGHT },
            { DEFAULT_WINDOW_WIDTH / 2, 0 }, { DEFAULT_WINDOW_WIDTH / 2, DEFAULT_WINDOW_HEIGHT },
            Engine::PrimitiveTopology::Triangles);

        pipeline2->SetVertexBuffer(vertexBuffer2);
        pipeline2->SetIndexBuffer(indexBuffer);

        pipeline2->SetModelMatrix(model);
        pipeline2->SetViewMatrix(view);
        pipeline2->SetProjectionMatrix(projection);

        float time = 0;

        // Initialize the thread pool
        //m_threadPool.Init();

        // Restart the clocks so that the first frame's delta time
        // is the lowest possible
        m_clock.restart();

        //m_currentScene = std::make_unique<MainMenuScene>();
        //m_currentScene->Init();

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

            time += delta_time;
            float xpos = std::cos(time * 2.0f) * 5.0f;
            float ypos = std::sin(time * 2.0f) * 5.0f;
            float zpos = std::cos(time * 2.0f) * 5.0f;

            float yrot = std::sin(time * 0.60f) * 180.0f;
            float xrot = std::cos(time * 1.0f) * 180.0f;
            float zrot = std::cos(time * 0.75f) * 180.0f;

            view = Engine::Matrix4<float>::LookAt({xpos, ypos, zpos}, objectPosition, { 0.0f, 1.0f, 0.0f });
            model = Engine::Matrix4<float>::Translation(objectPosition) * Engine::Matrix4<float>::Rotation({xrot, yrot, 45.0f + zrot});
            pipeline->SetModelMatrix(model);
            pipeline->SetViewMatrix(view);
            pipeline->Render();

            pipeline2->SetModelMatrix(model);
            pipeline2->SetViewMatrix(view);
            pipeline2->Render();

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