//
// Created by Killian on 14/03/2023.
//
#include <Application.h>
#include <RandomNumberGenerator.h>
#include <MainMenuScene.h>

std::unique_ptr<Application> Application::s_instance;

Application::Application()
{
    // Initialize the subsystems
    RandomNumberGenerator::Init();

    // Create and configure the window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    m_window.create(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), WINDOW_TITLE, sf::Style::Default, settings);
    m_window.setVerticalSyncEnabled(true);

    m_contextId = wglGetCurrentContext();
}

void Application::RunMainLoop()
{
    try
    {
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
                SPDLOG_INFO("FPS: {}", frames);
                frames = 0;
            }

            // Update and render the frame
            Update(delta_time);
            Render();

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
}

void Application::Update(float deltaTime)
{
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