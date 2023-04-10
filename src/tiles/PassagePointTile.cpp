//
// Created by Killian on 28/03/2023.
//
#include <Tiles.h>

PassagePointTile::PassagePointTile(uint64_t textureIndex, sf::Vector2f position, bool collidable, const uint8_t* data, uint32_t size) : Tile(textureIndex, position, collidable)
{

}

void PassagePointTile::Update(float deltaTime, GameGrid& grid, const std::shared_ptr<Player>& player)
{

}