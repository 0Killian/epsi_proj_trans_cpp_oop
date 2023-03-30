//
// Created by Killian on 16/03/2023.
//

#pragma once

#include <Scene.h>
#include <queue>
#include "ResourceRegistry.h"
#include "GameGrid.h"
#include "Player.h"

// A temporary scene

class MainMenuScene : public Scene
{
public:
    MainMenuScene();
    ~MainMenuScene() override;

    void Init() override;
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
    void HandleEvent(const sf::Event& event) override;

private:
    std::atomic_bool m_loaded = false;

    TextureRegistry::ResourceHandle m_loadingScreenTexture;
    sf::Sprite m_loadingScreenSprite;

    TextureRegistry::ResourceHandle m_mainMenuTexture;
    sf::Sprite m_mainMenuSprite;

    std::unique_ptr<GameGrid> m_testGameGrid;

    sf::Vector2f m_cameraMovement;
    sf::Vector2f m_pos;
    float m_zoomDelta = 0.0f;
    float m_zoom = 1.0f;

    Player m_player;

    std::queue<std::exception_ptr> m_exceptions;
};