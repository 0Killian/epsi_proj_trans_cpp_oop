//
// Created by Killian on 28/03/2023.
//
#include <Tiles.h>

PathTile::PathTile(uint64_t textureIndex, sf::Vector2f position, bool collidable) : Tile(textureIndex, position, collidable)
{

}

void PathTile::Update(float deltaTime, GameGrid& grid, const std::shared_ptr<Player>& player)
{

}