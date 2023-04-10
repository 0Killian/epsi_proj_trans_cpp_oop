//
// Created by Killian on 31/03/2023.
//
#include "GUILayer.h"

void GUILayer::Init()
{
    for(auto& guiObject : m_guiObjects)
    {
        guiObject->Init();
    }
}

void GUILayer::Update(float deltaTime)
{
    for(auto& guiObject : m_guiObjects)
    {
        guiObject->Update(deltaTime);
    }
}

void GUILayer::Render(sf::RenderWindow& window)
{
    // Reset the view to the default one (the one that covers the whole window)
    window.setView(window.getDefaultView());

    for(auto& guiObject : m_guiObjects)
    {
        guiObject->Render(window);
    }
}

bool GUILayer::HandleEvent(const sf::Event& event)
{
    for(auto& guiObject : m_guiObjects)
    {
        if(guiObject->HandleEvent(event)) return true;
    }
}