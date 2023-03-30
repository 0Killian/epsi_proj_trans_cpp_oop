//
// Created by Killian on 28/03/2023.
//
#pragma once

#include <GameGrid.h>

////////////////////////////////////////////////////////////
/// \brief  A tile that represents ground
///
/// This tile has no special effects. The player wan walk on it
///
////////////////////////////////////////////////////////////
class GroundTile : public GameGrid::Tile
{
public:
    explicit GroundTile(uint64_t textureIndex);

    void Update(float deltaTime, GameGrid& grid, const std::shared_ptr<Player>& player) override;
};

////////////////////////////////////////////////////////////
/// \brief  A tile that represents a wall
///
/// This tile has no special effects. The player cannot walk on it
///
////////////////////////////////////////////////////////////
class WallTile : public GameGrid::Tile
{
public:
    explicit WallTile(uint64_t textureIndex);

    void Update(float deltaTime, GameGrid& grid, const std::shared_ptr<Player>& player) override;
};

////////////////////////////////////////////////////////////
/// \brief  A tile used to teleport the player
///
/// This tile will teleport the player to another tile or
/// tilemap. This can be used to create doors or portals for
/// exemple.
///
////////////////////////////////////////////////////////////
class PassagePointTile : public GameGrid::Tile
{
public:
    PassagePointTile(uint64_t textureIndex, const uint8_t* data, uint32_t size);

    void Update(float deltaTime, GameGrid& grid, const std::shared_ptr<Player>& player) override;
};

////////////////////////////////////////////////////////////
/// \brief  A tile that represents a path
///
/// This tile will speed up the player. The NPCs will also
/// prefer to walk on it if possible.
///
////////////////////////////////////////////////////////////
class PathTile : public GameGrid::Tile
{
public:
    explicit PathTile(uint64_t textureIndex);

    void Update(float deltaTime, GameGrid& grid, const std::shared_ptr<Player>& player) override;
};

////////////////////////////////////////////////////////////
/// \brief  A tile that represents a tileable soil
///
/// This tile will slow down the player, but it is the only tile
/// that lets the player plant things.
///
////////////////////////////////////////////////////////////
class SoilTile : public GameGrid::Tile
{
public:
    SoilTile(uint64_t textureIndex, const uint8_t* data, uint32_t size);

    void Update(float deltaTime, GameGrid& grid, const std::shared_ptr<Player>& player) override;
};