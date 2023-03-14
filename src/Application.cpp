//
// Created by Killian on 14/03/2023.
//
#include <Application.h>
#include <RandomNumberGenerator.h>

std::unique_ptr<Application> Application::s_instance;

Application::Application()
{
    // Initialize the subsystems
    RandomNumberGenerator::Init();

    // Create and configure the window
    m_window.create(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), WINDOW_TITLE);
    m_window.setVerticalSyncEnabled(true);
}

Application::~Application() = default;

void Application::RunMainLoop()
{
    try
    {
        // Restart the clocks so that the first frame's delta time
        // is the lowest possible
        m_clock.restart();

        // Initialize all the objects
        m_circleColorChangeTime = CIRCLE_COLOR_CHANGE_TIME;

        m_circleShape.setRadius(CIRCLE_RADIUS);
        m_circleShape.setOrigin(sf::Vector2<float>(CIRCLE_RADIUS, CIRCLE_RADIUS));

        m_cursorCircleShape.setFillColor(sf::Color::Red);
        m_cursorCircleShape.setOrigin(sf::Vector2<float>(CURSOR_CIRCLE_RADIUS, CURSOR_CIRCLE_RADIUS));

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
        else if(event.type == sf::Event::MouseMoved)
        {
            // Move the rotation center of the circle to the mouse position
            m_circleRotationCenter = {
                static_cast<float>(event.mouseMove.x),
                static_cast<float>(event.mouseMove.y)
            };

            m_cursorCircleShape.setPosition(m_circleRotationCenter);
        }
    }

    // In case the window was closed without an event notifying it
    if(!m_window.isOpen())
    {
        throw Exception("Window closed unexpectedly");
    }

    // Update the angular displacement of the circle
    m_circleAngularDisplacement += CIRCLE_ANGULAR_SPEED * deltaTime;
    if(m_circleAngularDisplacement >= 360.0f * M_PI / 180.0f)
    {
        m_circleAngularDisplacement -= 360.0f * M_PI / 180.0f;
    }

    // Calculate the position of the circle using trigonometric
    // functions
    m_circleShape.setPosition(sf::Vector2<float>(
        std::cos(m_circleAngularDisplacement) * CIRCLE_DISTANCE_FROM_CENTER + m_circleRotationCenter.x,
        std::sin(m_circleAngularDisplacement) * CIRCLE_DISTANCE_FROM_CENTER + m_circleRotationCenter.y
    ));

    // Change the target color every CIRCLE_COLOR_CHANGE_TIME seconds
    m_circleColorChangeTime -= deltaTime;
    if(m_circleColorChangeTime <= 0.0f)
    {
        // Generate a random color
        m_circleTargetColor = sf::Color(
            static_cast<uint8_t>(RandomNumberGenerator::GetRandomInt(0, 255)),
            static_cast<uint8_t>(RandomNumberGenerator::GetRandomInt(0, 255)),
            static_cast<uint8_t>(RandomNumberGenerator::GetRandomInt(0, 255))
        );

        m_circleColorChangeTime = CIRCLE_COLOR_CHANGE_TIME;
    }

    // Calculate the delta between the current and the target color
    auto deltaR = static_cast<double>(m_circleTargetColor.r) - m_circleCurrentColor.x;
    auto deltaG = static_cast<double>(m_circleTargetColor.g) - m_circleCurrentColor.y;
    auto deltaB = static_cast<double>(m_circleTargetColor.b) - m_circleCurrentColor.z;

    // Update the current color to change it gradually in the CIRCLE_COLOR_CHANGE_TIME
    // period
    m_circleCurrentColor.x += deltaR * deltaTime / CIRCLE_COLOR_CHANGE_TIME;
    m_circleCurrentColor.y += deltaG * deltaTime / CIRCLE_COLOR_CHANGE_TIME;
    m_circleCurrentColor.z += deltaB * deltaTime / CIRCLE_COLOR_CHANGE_TIME;

    m_circleShape.setFillColor(sf::Color(
        static_cast<uint8_t>(m_circleCurrentColor.x),
        static_cast<uint8_t>(m_circleCurrentColor.y),
        static_cast<uint8_t>(m_circleCurrentColor.z)
    ));
}

void Application::Render()
{
    // Clear the window
    m_window.clear();

    // Draw the objects
    m_window.draw(m_cursorCircleShape);
    m_window.draw(m_circleShape);

    // Display the frame to the screen
    m_window.display();
}