//
// Created by Killian on 28/03/2023.
//
#include <Tiles.h>

SoilTile::SoilTile(uint64_t textureIndex, sf::Vector2f position, const uint8_t* data, uint32_t size) : Tile(textureIndex, position)
{

}

void SoilTile::Update(float deltaTime, GameGrid& grid, const std::shared_ptr<Player>& player)
{

}