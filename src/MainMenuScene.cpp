//
// Created by Killian and Alban on 16/03/2023.
//
#include <MainMenuScene.h>
#include <Application.h>
#include <GameGrid.h>

MainMenuScene::MainMenuScene() : Scene("MainMenuScene") {
}

void MainMenuScene::Init() {
    m_loadingScreenTexture = Application::GetInstance().GetTextureRegistry().GetResource("chargement.png");
    m_loadingScreenSprite.setTexture(m_loadingScreenTexture);

    sf::Vector2u loadingScreenSize = static_cast<const sf::Texture &>(m_loadingScreenTexture).getSize();
    loadingScreenSize.y /= 7;
    sf::Vector2u windowSize = sf::Vector2u(Application::WINDOW_WIDTH, Application::WINDOW_HEIGHT);
    sf::Vector2f scale = sf::Vector2f(
            static_cast<float>(windowSize.x) / static_cast<float>(loadingScreenSize.x),
            static_cast<float>(windowSize.y) / static_cast<float>(loadingScreenSize.y)
    );

    m_loadingScreenSprite.setScale({std::max(scale.x, scale.y), std::max(scale.x, scale.y)});
    m_loadingScreenSprite.setOrigin({loadingScreenSize.x / 2.0f, loadingScreenSize.y / 2.0f});
    m_loadingScreenSprite.setPosition({windowSize.x / 2.0f, windowSize.y / 2.0f});
    m_loadingScreenSprite.setTextureRect(sf::IntRect({0, 0}, {1920, 1080}));

    Application::GetInstance().GetThreadPool().Enqueue([this]() {
        try {
            SPDLOG_INFO("Initializing MainMenuScene...");

            m_mainMenuTexture = Application::GetInstance().GetTextureRegistry().GetResource("main_menu.png");
            m_mainMenuSprite.setTexture(m_mainMenuTexture);

            sf::Vector2u mainMenuSize = static_cast<const sf::Texture &>(m_mainMenuTexture).getSize();
            sf::Vector2u windowSize = sf::Vector2u(Application::WINDOW_WIDTH, Application::WINDOW_HEIGHT);
            sf::Vector2f scale = sf::Vector2f(
                    static_cast<float>(windowSize.x) / static_cast<float>(mainMenuSize.x),
                    static_cast<float>(windowSize.y) / static_cast<float>(mainMenuSize.y)
            );

            m_mainMenuSprite.setScale({std::max(scale.x, scale.y), std::max(scale.x, scale.y)});
            m_mainMenuSprite.setOrigin({mainMenuSize.x / 2.0f, mainMenuSize.y / 2.0f});
            m_mainMenuSprite.setPosition({windowSize.x / 2.0f, windowSize.y / 2.0f});
            m_mainMenuSprite.setTextureRect(sf::IntRect(
                {0, 0},
                {static_cast<int>(mainMenuSize.x), static_cast<int>(mainMenuSize.y)}
            ));

            m_testGameGrid = GameGrid::ReadFromFile("assets/tilemaps/tilemap.htf");

            for (int i = 0; i < 100; i++) {
                SPDLOG_INFO("Initializing GameObject {}...", i);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                int index = static_cast<int>(7.0f / (100.0f / static_cast<float>(i + 1)));
                SPDLOG_INFO("Index: {}", index);
                m_loadingScreenSprite.setTextureRect(sf::IntRect({0, 1080 * index}, {1920, 1080}));
            }

            SPDLOG_INFO("MainMenuScene initialized!");
            m_loaded = true;
        }
        catch (std::exception &e) {
            m_exceptions.push(std::current_exception());
        }
    });

}

void MainMenuScene::Update(float deltaTime) {
    if (!m_loaded && !m_exceptions.empty()) {
        std::rethrow_exception(m_exceptions.front());
    }

    if (m_loaded) {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            m_pos.x -= 2 * deltaTime;
            m_testGameGrid->SetCameraPosition(m_pos);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            m_pos.x += 2 * deltaTime;
            m_testGameGrid->SetCameraPosition(m_pos);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            m_pos.y -= 2 * deltaTime;
            m_testGameGrid->SetCameraPosition(m_pos);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            m_pos.y += 2 * deltaTime;
            m_testGameGrid->SetCameraPosition(m_pos);
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            m_zoom += 0.1f * deltaTime;
            m_testGameGrid->SetCameraZoom(m_zoom);
        }
        else if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            m_zoom -= 0.1f * deltaTime;
            m_testGameGrid->SetCameraZoom(m_zoom);
        }

        m_testGameGrid->Update(deltaTime);
    }
}

void MainMenuScene::Render(sf::RenderWindow &window) {
    if (!m_loaded) {
        // Loading Screen
        window.draw(m_loadingScreenSprite);
        window.draw(m_loadingCircle);
    } else {
        // Main Menu
        //window.draw(m_mainMenuSprite);
        m_testGameGrid->Render(window);
    }
}

MainMenuScene::~MainMenuScene() = default;