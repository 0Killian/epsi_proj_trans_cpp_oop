//
// Created by Killian on 14/03/2023.
//

#pragma once

#include <memory>
#include <utility>


////////////////////////////////////////////////////////////
/// \brief  A singleton class which defines the application
///
////////////////////////////////////////////////////////////
class Application {
public:
    ////////////////////////////////////////////////////////////
    /// \brief  Deleted copy constructor
    ///
    ////////////////////////////////////////////////////////////
    Application(const Application&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief  Deleted move constructor
    ///
    ////////////////////////////////////////////////////////////
    Application(Application&&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief  Deleted copy assignment operator
    ///
    ////////////////////////////////////////////////////////////
    Application& operator=(const Application&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief  Deleted move assignment operator
    ///
    ////////////////////////////////////////////////////////////
    Application& operator=(Application&&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief  Destructor
    ///
    /// Closes the application window
    ///
    ////////////////////////////////////////////////////////////
    ~Application();


    ////////////////////////////////////////////////////////////
    /// \brief  Returns the instance of the application
    ///
    /// When this function is called for the first time, it will create
    /// the application
    ///
    /// \return A reference to the application
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static inline Application& GetInstance()
    {
        if (!s_instance)
        {
            s_instance = std::unique_ptr<Application>(new Application());
        }

        return *s_instance;
    }

    ////////////////////////////////////////////////////////////
    /// \brief  Starts the mainloop and run it, until the user closes
    ///         the application or an error occurs
    ///
    ////////////////////////////////////////////////////////////
    void RunMainLoop();

    ////////////////////////////////////////////////////////////
    /// \brief  An exception class used by the Application
    ///
    ////////////////////////////////////////////////////////////
    class Exception : public std::exception
    {
    public:
        ////////////////////////////////////////////////////////////
        /// \brief  Constructor
        ///
        /// \param message A description of the exception
        ///
        ////////////////////////////////////////////////////////////
        explicit inline Exception(const std::string& message)
            : m_message(std::string("[APPLICATION EXCEPTION] " + message))
        {}

        ////////////////////////////////////////////////////////////
        /// \brief  Returns the description of the exception
        ///
        /// \return The string containing the message
        ///
        ///////////////////////////////////////////////////////////////
        [[nodiscard]] inline const char* what() const noexcept override
        {
            return m_message.c_str();
        }

    private:
        ////////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////
        std::string m_message;
    };

private:
    ////////////////////////////////////////////////////////////
    /// \brief  The default constructor
    ///
    /// Creates the window and initializes the subsystems (RNG,
    /// Networking, Audio...)
    ///
    ////////////////////////////////////////////////////////////
    Application();

    ////////////////////////////////////////////////////////////
    /// \brief  Update the frame based on the time the last frame took
    ///
    /// All physics calculations and logic should happen here.
    ///
    /// \param deltaTime The time in seconds the last frame took
    ///
    ////////////////////////////////////////////////////////////
    void Update(float deltaTime);

    ////////////////////////////////////////////////////////////
    /// \brief  Renders the frame to the screen
    ///
    /// All rendering calls should happen here
    ///
    ////////////////////////////////////////////////////////////
    void Render();

    ////////////////////////////////////////////////////////////
    // Constants
    ////////////////////////////////////////////////////////////
    static constexpr const char* WINDOW_TITLE = "Stardew";
    static constexpr uint32_t WINDOW_WIDTH = 800;
    static constexpr uint32_t WINDOW_HEIGHT = 600;

    static constexpr float CIRCLE_RADIUS = 100.0f;
    static constexpr float CIRCLE_ANGULAR_SPEED = 180.0f * M_PI / 180.0f;
    static constexpr float CIRCLE_DISTANCE_FROM_CENTER = 200.0f;
    static constexpr float CIRCLE_COLOR_CHANGE_TIME = 2.0f;

    static constexpr float CURSOR_CIRCLE_RADIUS = 10.0f;

    ////////////////////////////////////////////////////////////
    // Static data
    ////////////////////////////////////////////////////////////
    static std::unique_ptr<Application> s_instance;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    bool m_running = true;
    sf::RenderWindow m_window;

    float m_circleAngularDisplacement = 0.0f;
    sf::Vector2<float> m_circleRotationCenter = { 0.0f, 0.0f };
    sf::CircleShape m_circleShape = sf::CircleShape(CIRCLE_RADIUS);
    sf::Color m_circleTargetColor = sf::Color::White;
    sf::Vector3<double> m_circleCurrentColor = { 0.0f, 0.0f, 0.0f };
    float m_circleColorChangeTime = 0.0f;

    sf::CircleShape m_cursorCircleShape = sf::CircleShape(CURSOR_CIRCLE_RADIUS);

    sf::Clock m_clock;
    float m_timer = 0.0f;
    uint16_t frames = 0;
};
