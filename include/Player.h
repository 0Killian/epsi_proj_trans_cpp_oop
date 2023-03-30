//
// Created by Alban on 30/03/2023.
//
#pragma once

#include <GameObject.h>
#include "ResourceRegistry.h"

class Player : public GameObject{
public:
    void Init() override;
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
    inline void SetZoomFactor(float zoomFactor){m_zoomFactor = zoomFactor;};


private:
    float m_zoomFactor = 1;
    TextureRegistry::ResourceHandle m_texture;
    sf::Sprite m_sprite;
};