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
    [[nodiscard]] bool HandleEvent(const sf::Event& event) override;

    inline void SetZoomFactor(float zoomFactor) override { m_zoomFactor = zoomFactor; };
    inline void SetPosition(sf::Vector2f position) { m_position = position; }

    [[nodiscard]] inline sf::Vector2f GetPosition() const { return m_position; };
    [[nodiscard]] inline float GetZoomFactor() const { return m_zoomFactor; }
    [[nodiscard]] sf::Rect<float> GetBoundingBox() const;

private:
    static constexpr float ANIMATION_DELAY = 0.1f;
    static constexpr int ANIMATION_FRAME_COUNT = 6;
    static constexpr int WIDTH = 32;
    static constexpr int HEIGHT = 64;

    Orientation m_orientation = DOWN;
    float m_animationTimer = 0.0f;
    bool m_isMoving = false;

    sf::Vector2f m_movement;
    sf::Vector2f m_position;

    float m_zoomDelta = 0.0f;
    float m_zoomFactor = 1.0f;

    TextureRegistry::ResourceHandle m_texture;
    sf::Sprite m_sprite;

    void DoAnimation(int index, float timer);
};