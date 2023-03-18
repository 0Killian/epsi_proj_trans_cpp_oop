//
// Created by Killian on 14/03/2023.
//

#pragma once

#include <memory>
#include <utility>
#include <Scene.h>
#include <ResourceRegistry.h>
#include <ThreadPool.h>

////////////////////////////////////////////////////////////
/// \brief  A singleton class which defines the application
///
////////////////////////////////////////////////////////////
class Application {
public:
    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

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
    /// \brief  Starts the main loop and run it, until the user closes
    ///         the application or an error occurs
    ///
    ////////////////////////////////////////////////////////////
    void RunMainLoop();

    ////////////////////////////////////////////////////////////
    /// \brief  Returns the texture registry
    ///
    ///
    /// The texture registry is used to load and store textures.
    ///
    /// \return A reference to the texture registry
    ///
    /// \see TextureRegistry
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] inline TextureRegistry& GetTextureRegistry() { return m_textureRegistry; }

    ////////////////////////////////////////////////////////////
    /// \brief  Returns the the thread pool
    ///
    /// The thread pool is used to execute tasks asynchronously.
    ///
    /// \return A reference to the thread pool
    ///
    /// \see ThreadPool
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] inline ThreadPool& GetThreadPool() { return m_threadPool; }

    static constexpr const char* WINDOW_TITLE = "Stardew";
    static constexpr uint32_t WINDOW_WIDTH = 800;
    static constexpr uint32_t WINDOW_HEIGHT = 600;

protected:
    friend ThreadPool;

    ////////////////////////////////////////////////////////////
    /// \brief  Returns the context id
    ///
    /// The context id is used to identify the OpenGL context.
    /// This function is only used internally.
    ///
    /// \return The context id
    ///
    ////////////////////////////////////////////////////////////
#ifdef _WIN32
    [[nodiscard]] HGLRC GetContextId() { return m_contextId; }
#else
#error Not Supported!
#endif

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

    static constexpr float CIRCLE_RADIUS = 100.0f;
    static constexpr float CIRCLE_ANGULAR_SPEED = 180.0f * M_PI / 180.0f;
    static constexpr float CIRCLE_DISTANCE_FROM_CENTER = 200.0f;
    static constexpr float CIRCLE_COLOR_CHANGE_TIME = 2.0f;

    static constexpr float CURSOR_CIRCLE_RADIUS = 10.0f;

    static std::unique_ptr<Application> s_instance;

    bool m_running = true;
    sf::RenderWindow m_window;

    TextureRegistry m_textureRegistry;

    sf::Clock m_clock;
    float m_timer = 0.0f;
    uint16_t frames = 0;

    std::unique_ptr<Scene> m_currentScene;

    ThreadPool m_threadPool;

#ifdef _WIN32
    HGLRC m_contextId;
#else
#error Not Supported!
#endif
};
