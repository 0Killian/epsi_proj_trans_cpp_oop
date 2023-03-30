//
// Created by Alban on 30/03/2023.
//
#pragma once

#include <GameObject.h>
#include "ResourceRegistry.h"

enum Orientation {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

class Player : public GameObject{
public:
    void Init() override;
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
    inline void SetZoomFactor(float zoomFactor){m_zoomFactor = zoomFactor;};
    inline void SetOrientation(Orientation orientation) {m_orientation = orientation;};
    inline void SetPosition(sf::Vector2f position) { m_position = position; }
    [[nodiscard]] inline sf::Vector2f GetPosition() const { return m_position; };

private:
    float m_zoomFactor = 1;
    Orientation m_orientation = DOWN;

    sf::Vector2f m_position;

    TextureRegistry::ResourceHandle m_texture;
    sf::Sprite m_sprite;
};