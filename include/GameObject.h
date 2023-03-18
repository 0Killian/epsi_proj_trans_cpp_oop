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
    virtual ~GameObject() = default;

    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
};