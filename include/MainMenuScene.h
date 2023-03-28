//
// Created by Killian on 16/03/2023.
//

#pragma once

#include <Scene.h>
#include <queue>
#include "ResourceRegistry.h"
#include "GameGrid.h"

// A temporary scene

class MainMenuScene : public Scene
{
public:
    MainMenuScene();
    ~MainMenuScene() override;

    void Init() override;
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;

private:
    std::atomic_bool m_loaded = false;
    sf::CircleShape m_loadingCircle;

    TextureRegistry::ResourceHandle m_loadingScreenTexture;
    sf::Sprite m_loadingScreenSprite;

    TextureRegistry::ResourceHandle m_mainMenuTexture;
    sf::Sprite m_mainMenuSprite;

    std::unique_ptr<GameGrid> m_testGameGrid;

    std::queue<std::exception_ptr> m_exceptions;

    sf::Vector2f m_pos = {0, 0};
    float m_zoom = 1.0f;
};