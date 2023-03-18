//
// Created by Killian on 16/03/2023.
//

#pragma once

#include <Scene.h>
#include <queue>
#include "ResourceRegistry.h"

// A temporary scene

class MainMenuScene : public Scene
{
public:
    MainMenuScene();
    ~MainMenuScene() override;

    void Init() override;
    void Update() override;
    void Render(sf::RenderWindow& window) override;

private:
    std::atomic_bool m_loaded = false;
    sf::CircleShape m_loadingCircle;

    TextureRegistry::ResourceHandle m_loadingScreenTexture;
    sf::Sprite m_loadingScreenSprite;

    TextureRegistry::ResourceHandle m_mainMenuTexture;
    sf::Sprite m_mainMenuSprite;

    std::queue<std::exception_ptr> m_exceptions;
};