//
// Created by Killian on 28/03/2023.
//
#include <Tiles.h>

WallTile::WallTile(uint64_t textureIndex, sf::Vector2f position) : Tile(textureIndex, position)
{

}

void WallTile::Update(float deltaTime, GameGrid& grid, const std::shared_ptr<Player>& player)
{

}