//
// Created by Killian on 12/04/2023.
//
#include <Application.h>
#include "items/Apple.h"
#include "Player.h"
#include "GameGrid.h"

Apple::Apple() : Item(ItemId::APPLE)
{
    m_texture = Application::GetInstance().GetTextureRegistry().GetResource("items/apple.png");
}

void Apple::OnUse(Player &player)
{
    player.TileArea(sf::IntRect{ player.GetTilePosition(), {1, 1} });
}