//
// Created by Killian on 15/03/2023.
//

#pragma once

////////////////////////////////////////////////////////////
/// \brief  A base class for all game objects
///
////////////////////////////////////////////////////////////
class GameObject
{
public:
    virtual ~GameObject() = default;

    virtual void Init() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render(sf::RenderWindow& window) = 0;
    virtual void SetZoomFactor(float zoomFactor) = 0;
};