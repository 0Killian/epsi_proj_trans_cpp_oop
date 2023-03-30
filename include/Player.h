//
// Created by Alban on 30/03/2023.
//
#pragma once

#include <GameObject.h>
#include "ResourceRegistry.h"

enum Orientation : int {
    UP = 1,
    RIGHT = 2,
    DOWN = 0,
    LEFT = 3
};

class Player : public GameObject{
public:
    void Init() override;
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
    inline void SetZoomFactor(float zoomFactor) override { m_zoomFactor = zoomFactor; };
    inline void SetOrientation(Orientation orientation) { m_orientation = orientation; };
    inline void SetIsMoving(bool isMoving) { m_isMoving = isMoving; };
    inline void SetPosition(sf::Vector2f position) { m_position = position; }
    [[nodiscard]] inline sf::Vector2f GetPosition() const { return m_position; };

    [[nodiscard]] sf::Rect<float> GetBoundingBox() const;

private:
    static constexpr float ANIMATION_DELAY = 0.1f;
    static constexpr int ANIMATION_FRAME_COUNT = 6;
    static constexpr int WIDTH = 32;
    static constexpr int HEIGHT = 64;

    float m_animationTimer = 0.0f;
    Orientation m_orientation = DOWN;
    bool m_isMoving = false;

    float m_zoomFactor = 1;
    sf::Vector2f m_position;

    TextureRegistry::ResourceHandle m_texture;
    sf::Sprite m_sprite;

    void DoAnimation(int index, float timer);
};