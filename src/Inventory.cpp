//
// Created by Alban on 31/03/2023.
//

#include "Item.h"
#include "Inventory.h"
#include <Player.h>
#include <Application.h>

void Inventory::Init()
{
    m_texture = Application::GetInstance().GetTextureRegistry().GetResource("inventory.png");
    m_inventory.setTexture(m_texture);
    // Center sprite on the screen
    m_inventory.setOrigin({16.0f, 48.0f});
    m_inventory.setPosition({Application::WINDOW_WIDTH / 2.f, Application::WINDOW_HEIGHT / 2.f});
    m_inventory.setTextureRect(sf::IntRect({0, 0}, {32, 64}));
    count = 0;
}

Inventory::Inventory() {}

void Inventory::addItem(const Item& item) {
    if (count < 45) { // vérifie si la table n'est pas pleine
        items[count] = item;
        count++;
    }
}

void Inventory::removeItem(int index) {
    if (index >= 0 && index < count) { // vérifie si l'index est valide
        for (int i = index; i < count - 1; i++) {
            items[i] = items[i + 1];
        }
        count--;
    }
}

void Inventory::Update(float deltaTime)
{

}

void Inventory::Render(sf::RenderWindow& window)
{
    window.draw(m_inventory);
}