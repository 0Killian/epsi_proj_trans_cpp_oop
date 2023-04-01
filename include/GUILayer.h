
//
// Created by Killian on 31/03/2023.
//

#pragma once

#include "GameObject.h"

class GUILayer : public GameObject
{
public:
    void Init() override;
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
    bool HandleEvent(const sf::Event& event) override;

private:
    std::vector<std::shared_ptr<GameObject>> m_guiObjects;
};
