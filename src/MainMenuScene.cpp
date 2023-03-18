//
// Created by Killian on 16/03/2023.
//
#include <MainMenuScene.h>
#include <Application.h>

MainMenuScene::MainMenuScene() : Scene("MainMenuScene")
{
}

void MainMenuScene::Init()
{
    m_loadingScreenTexture = Application::GetInstance().GetTextureRegistry().GetResource("loading_screen.png");
    m_loadingScreenSprite.setTexture(m_loadingScreenTexture);
    sf::Vector2u loadingScreenSize = static_cast<const sf::Texture&>(m_loadingScreenTexture).getSize();
    sf::Vector2u windowSize = sf::Vector2u(Application::WINDOW_WIDTH, Application::WINDOW_HEIGHT);
    sf::Vector2f scale = sf::Vector2f(
        static_cast<float>(windowSize.x) / static_cast<float>(loadingScreenSize.x),
        static_cast<float>(windowSize.y) / static_cast<float>(loadingScreenSize.y)
    );
    m_loadingScreenSprite.setScale(scale);

    Application::GetInstance().GetThreadPool().Enqueue([this]()
    {
        try
        {
            SPDLOG_INFO("Initializing MainMenuScene...");

            m_mainMenuTexture = Application::GetInstance().GetTextureRegistry().GetResource("main_menu.png");

            sf::Vector2u mainMenuTexture = static_cast<const sf::Texture &>(m_mainMenuTexture).getSize();
            sf::Vector2u windowSize = sf::Vector2u(Application::WINDOW_WIDTH, Application::WINDOW_HEIGHT);
            sf::Vector2f scale = sf::Vector2f(
                    static_cast<float>(windowSize.x) / static_cast<float>(mainMenuTexture.x),
                    static_cast<float>(windowSize.y) / static_cast<float>(mainMenuTexture.y)
            );
            m_mainMenuSprite.setScale(scale);
            m_mainMenuSprite.setTexture(m_mainMenuTexture);

            for (int i = 0; i < 100; i++)
            {
                SPDLOG_INFO("Initializing GameObject {}...", i);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            SPDLOG_INFO("MainMenuScene initialized!");

            m_loaded = true;
        }
        catch(...)
        {
            m_exceptions.push(std::current_exception());
        }
    });
}

void MainMenuScene::Update()
{
    if(!m_loaded && !m_exceptions.empty())
    {
        std::rethrow_exception(m_exceptions.front());
    }
}

void MainMenuScene::Render(sf::RenderWindow& window)
{
    if(!m_loaded)
    {
        // Loading Screen
        window.draw(m_loadingScreenSprite);
        window.draw(m_loadingCircle);
    }
    else
    {
        // Main Menu
        window.draw(m_mainMenuSprite);
    }
}

MainMenuScene::~MainMenuScene()
{
}