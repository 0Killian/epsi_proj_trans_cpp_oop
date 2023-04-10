//
// Created by Killian on 21/03/2023.
//
#include <Tiles.h>

GroundTile::GroundTile(uint64_t textureIndex, sf::Vector2f position, bool collidable) : Tile(textureIndex, position, collidable)
{

}

void GroundTile::Update(float deltaTime, GameGrid& grid, const std::shared_ptr<Player>& player)
{
    // Todo: create particle when walking on it
}