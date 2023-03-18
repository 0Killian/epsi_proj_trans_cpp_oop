//
// Created by Killian on 15/03/2023.
//

#pragma once

#include <memory>
#include <vector>
#include <GameObject.h>

////////////////////////////////////////////////////////////
/// \brief  The base class for scenes
///
/// A scene is sort of a level. It contains all the game
/// objects and is responsible for updating and rendering
/// them.
///
/// \see GameObject
///
////////////////////////////////////////////////////////////
class Scene
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief  The default constructor.
    ///
    /// \param name the name of the scene (only available on debug)
    ///
    ////////////////////////////////////////////////////////////
#ifdef DEBUG
    explicit Scene(const char* name) : m_name(name) {}
#else
    Scene() = default;
#endif
    virtual ~Scene() = default;

    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Render(sf::RenderWindow& window) = 0;

protected:
    std::vector<std::shared_ptr<GameObject>> m_gameObjects;
#ifdef DEBUG
    const char* m_name;
#endif
};
