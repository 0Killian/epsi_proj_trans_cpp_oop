//
// Created by Killian on 01/04/2023.
//

#pragma once

struct CollisionInfo
{
    bool collision = false;
    sf::Vector2f contactPoint = { 0.0f, 0.0f };
    sf::Vector2f contactNormal = { 0.0f, 0.0f };
    float penetrationDepth = 0.0f;

    [[nodiscard]] explicit operator bool() const
    {
        return collision;
    }
};

CollisionInfo RayRectangleCollision(const sf::Vector2f& rayOrigin, const sf::Vector2f& rayDirection, const sf::FloatRect& rect);
CollisionInfo DynamicRectangleCollision(const sf::FloatRect& rect1, const sf::FloatRect& rect2, const sf::Vector2f& collisionVector);