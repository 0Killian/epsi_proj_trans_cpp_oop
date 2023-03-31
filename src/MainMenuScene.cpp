//
// Created by Killian and Alban on 16/03/2023.
//
#include <MainMenuScene.h>
#include <Application.h>
#include <GameGrid.h>
#include <Player.h>
#include <Item.h>
#include <Inventory.h>

MainMenuScene::MainMenuScene() : Scene("MainMenuScene")
{
}

void MainMenuScene::Init()
{
    Item patate(1, "Patate", true, 99, 1, true, false);
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


    Application::GetInstance().GetThreadPool().Enqueue([this]()
    {
        try
        {
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
            if(m_testGameGrid == nullptr)
            {
                throw std::runtime_error("Failed to load tilemap !");
            }

            m_gameObjects.push_back(m_testGameGrid);

            m_player = std::make_shared<Player>();
            m_testGameGrid->SetPlayer(m_player);
            m_gameObjects.push_back(m_player);

            for (int i = 0; i < m_gameObjects.size(); i++)
            {
                m_gameObjects[i]->Init();
                float percentage = static_cast<float>(i + 1) / static_cast<float>(m_gameObjects.size()) * 100.0f;
                int index = static_cast<int>(7.0f / (100.0f / static_cast<float>(percentage + 1)));
                m_loadingScreenSprite.setTextureRect(sf::IntRect({0, 1080 * index}, {1920, 1080}));
            }

            m_testGameGrid->SetPlayer(m_player);

            m_loaded = true;
        }
        catch (std::exception &e) {
            m_exceptions.push(std::current_exception());
        }
    });
}

void MainMenuScene::HandleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Q || event.key.code == sf::Keyboard::Left)
        {
            m_movement.x -= 1;
        }
        else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right)
        {
            m_movement.x += 1;
        }
        else if (event.key.code == sf::Keyboard::Z || event.key.code == sf::Keyboard::Up)
        {
            m_movement.y -= 1;
        }
        else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
        {
            m_movement.y += 1;
        }
    }
    else if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::Q || event.key.code == sf::Keyboard::Left)
        {
            m_movement.x += 1;
        }
        else if(event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right)
        {
            m_movement.x -= 1;
        }
        else if (event.key.code == sf::Keyboard::Z || event.key.code == sf::Keyboard::Up)
        {
            m_movement.y += 1;
        }
        else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
        {
            m_movement.y -= 1;
        }
    }
    else if (event.type == sf::Event::MouseWheelScrolled)
    {
        m_zoomDelta = event.mouseWheelScroll.delta * 20;
    }
    else if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {

        }
    }
}

void MainMenuScene::Update(float deltaTime)
{
    if (!m_loaded && !m_exceptions.empty())
    {
        std::rethrow_exception(m_exceptions.front());
    }

    if (m_loaded)
    {
        sf::Vector2f move = sf::Vector2f(0, 0);
        if(m_movement != sf::Vector2f(0, 0))
        {
            m_player->SetIsMoving(true);
            move = m_movement.normalized() * GameGrid::TILE_SIZE * 2.0f * deltaTime;

            if(m_movement.x < 0)
            {
                m_player->SetOrientation(Orientation::LEFT);
            }
            else if(m_movement.x > 0)
            {
                m_player->SetOrientation(Orientation::RIGHT);
            }
            else if(m_movement.y < 0)
            {
                m_player->SetOrientation(Orientation::UP);
            }
            else if(m_movement.y > 0)
            {
                m_player->SetOrientation(Orientation::DOWN);
            }
        }
        else
        {
            m_player->SetIsMoving(false);
        }

        sf::Vector2f pos = m_player->GetPosition() + move;

        m_zoom += m_zoomDelta * deltaTime;
        m_zoomDelta = 0;

        if(m_zoom < 0.5f)
        {
            m_zoom = 0.5f;
        }

        if(m_zoom > 2.0f)
        {
            m_zoom = 2.0f;
        }

        m_player->SetZoomFactor(m_zoom);
        m_player->SetPosition(pos);

        m_testGameGrid->SetZoomFactor(m_zoom);

        for (const auto & m_gameObject : m_gameObjects)
        {
            m_gameObject->Update(deltaTime);
        }
    }
}


void MainMenuScene::Render(sf::RenderWindow &window) {
    if (!m_loaded) {
        // Loading Screen
        window.draw(m_loadingScreenSprite);
    } else {
        // Main Menu
        //window.draw(m_mainMenuSprite);
        for(const auto& m_gameObject : m_gameObjects)
        {
            m_gameObject->Render(window);
        }
    }
}

MainMenuScene::~MainMenuScene() = default;


