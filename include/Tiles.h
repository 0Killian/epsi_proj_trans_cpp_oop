//
// Created by Killian on 28/03/2023.
//
#pragma once

#include <GameGrid.h>

struct TextureCombination
{
    std::optional<bool> bottom;
    std::optional<bool> top;
    std::optional<bool> left;
    std::optional<bool> right;
    std::optional<bool> bottomLeft;
    std::optional<bool> bottomRight;
    std::optional<bool> topLeft;
    std::optional<bool> topRight;
    int textureIndex;
};

////////////////////////////////////////////////////////////
/// \brief  A tile that represents ground
///
/// This tile has no special effects.
///
////////////////////////////////////////////////////////////
class GroundTile : public GameGrid::Tile
{
public:
    explicit GroundTile(uint64_t textureIndex, sf::Vector2f position, bool collidable);

    void Update(float deltaTime, GameGrid& grid, const std::shared_ptr<Player>& player) override;
    [[nodiscard]] inline TileType GetType() const override { return TileType::Ground; }
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
    PassagePointTile(uint64_t textureIndex, sf::Vector2f position, bool collidable, const uint8_t* data, uint32_t size);

    void Update(float deltaTime, GameGrid& grid, const std::shared_ptr<Player>& player) override;
    [[nodiscard]] inline TileType GetType() const override { return TileType::PassagePoint; }
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
    explicit PathTile(uint64_t textureIndex, sf::Vector2f position, bool collidable);

    void Update(float deltaTime, GameGrid& grid, const std::shared_ptr<Player>& player) override;
    [[nodiscard]] inline TileType GetType() const override { return TileType::Path; }
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
    SoilTile(uint64_t textureIndex, sf::Vector2f position, bool collidable, const uint8_t* data, uint32_t size);

    void Update(float deltaTime, GameGrid& grid, const std::shared_ptr<Player>& player) override;
    [[nodiscard]] inline TileType GetType() const override { return TileType::Soil; }

    static constexpr int TEXTURE_ALL_CONNECTED = 43; // TODO
    static constexpr int TEXTURE_TOP_CONNECTED = 48;
    static constexpr int TEXTURE_BOTTOM_CONNECTED = 30;
    static constexpr int TEXTURE_LEFT_CONNECTED = 5;
    static constexpr int TEXTURE_RIGHT_CONNECTED = 3;
    static constexpr int TEXTURE_NONE_CONNECTED = 10;
    static constexpr int TEXTURE_BOTTOM_RIGHT_CONNECTED = 43; // TODO
    static constexpr int TEXTURE_BOTTOM_LEFT_CONNECTED = 43; // TODO
    static constexpr int TEXTURE_TOP_RIGHT_CONNECTED = 43; // TODO
    static constexpr int TEXTURE_TOP_LEFT_CONNECTED = 43; // TODO
    static constexpr int TEXTURE_LEFT_RIGHT_CONNECTED = 4;
    static constexpr int TEXTURE_TOP_BOTTOM_CONNECTED = 39;
    static constexpr int TEXTURE_TOP_BOTTOM_RIGHT_CONNECTED = 43; // TODO
    static constexpr int TEXTURE_TOP_BOTTOM_LEFT_CONNECTED = 43; // TODO
    static constexpr int TEXTURE_TOP_LEFT_RIGHT_CONNECTED = 43; // TODO
    static constexpr int TEXTURE_BOTTOM_LEFT_RIGHT_CONNECTED = 43; // TODO
    static constexpr int TEXTURE_ALL_CONNECTED_ALL_CORNERS = 16;
    static constexpr int TEXTURE_TOP_LEFT_CONNECTED_TOP_LEFT_CORNER = 26;
    static constexpr int TEXTURE_TOP_RIGHT_CONNECTED_TOP_RIGHT_CORNER = 24;
    static constexpr int TEXTURE_BOTTOM_RIGHT_CONNECTED_BOTTOM_RIGHT_CORNER = 6;
    static constexpr int TEXTURE_BOTTOM_LEFT_CONNECTED_BOTTOM_LEFT_CORNER = 8;
    static constexpr int TEXTURE_TOP_LEFT_RIGHT_CONNECTED_TOP_CORNER = 25;
    static constexpr int TEXTURE_TOP_BOTTOM_RIGHT_CONNECTED_RIGHT_CORNER = 15;
    static constexpr int TEXTURE_BOTTOM_LEFT_RIGHT_CONNECTED_BOTTOM_CORNER = 7;
    static constexpr int TEXTURE_TOP_BOTTOM_LEFT_CONNECTED_LEFT_CORNER = 17;
    static constexpr int TEXTURE_BOTTOM_LEFT_RIGHT_CONNECTED_BOTTOM_LEFT_CORNER = 43; // TODO
    static constexpr int TEXTURE_BOTTOM_LEFT_RIGHT_CONNECTED_BOTTOM_RIGHT_CORNER = 43; // TODO
    static constexpr int TEXTURE_TOP_LEFT_RIGHT_CONNECTED_TOP_RIGHT_CORNER = 43; // TODO
    static constexpr int TEXTURE_TOP_LEFT_RIGHT_CONNECTED_TOP_LEFT_CORNER = 43; // TODO
    static constexpr int TEXTURE_TOP_BOTTOM_RIGHT_CONNECTED_TOP_RIGHT_CORNER = 43; // TODO
    static constexpr int TEXTURE_TOP_BOTTOM_RIGHT_CONNECTED_TOP_LEFT_CORNER = 43; // TODO
    static constexpr int TEXTURE_TOP_BOTTOM_LEFT_CONNECTED_TOP_LEFT_CORNER = 43; // TODO
    static constexpr int TEXTURE_TOP_BOTTOM_LEFT_CONNECTED_TOP_RIGHT_CORNER = 43; // TODO
    static constexpr int TEXTURE_ALL_CONNECTED_BOTTOM_CORNER = 43; // TODO
    static constexpr int TEXTURE_ALL_CONNECTED_TOP_CORNER = 43; // TODO
    static constexpr int TEXTURE_ALL_CONNECTED_LEFT_CORNER = 43; // TODO
    static constexpr int TEXTURE_ALL_CONNECTED_RIGHT_CORNER = 43; // TODO
    static constexpr int TEXTURE_ALL_CONNECTED_TOP_RIGHT_CORNER = 43; // TODO
    static constexpr int TEXTURE_ALL_CONNECTED_TOP_LEFT_CORNER = 43; // TODO
    static constexpr int TEXTURE_ALL_CONNECTED_BOTTOM_RIGHT_CORNER = 43; // TODO
    static constexpr int TEXTURE_ALL_CONNECTED_BOTTOM_LEFT_CORNER = 43; // TODO

    static constexpr TextureCombination TEXTURES[41] = {
            {false, false, false, false, std::nullopt, std::nullopt, std::nullopt, std::nullopt, SoilTile::TEXTURE_NONE_CONNECTED},
            {false, false, true, false, std::nullopt, std::nullopt, std::nullopt, std::nullopt, SoilTile::TEXTURE_LEFT_CONNECTED},
            {false, false, false, true, std::nullopt, std::nullopt, std::nullopt, std::nullopt, SoilTile::TEXTURE_RIGHT_CONNECTED},
            {false, true, false, false, std::nullopt, std::nullopt, std::nullopt, std::nullopt, SoilTile::TEXTURE_TOP_CONNECTED},
            {true, false, false, false, std::nullopt, std::nullopt, std::nullopt, std::nullopt, SoilTile::TEXTURE_BOTTOM_CONNECTED},
            {true, false, false, true, std::nullopt, false, std::nullopt, std::nullopt, SoilTile::TEXTURE_BOTTOM_RIGHT_CONNECTED},
            {true, false, true, false, false, std::nullopt, std::nullopt, std::nullopt, SoilTile::TEXTURE_BOTTOM_LEFT_CONNECTED},
            {false, true, false, true, std::nullopt, std::nullopt, std::nullopt, false, SoilTile::TEXTURE_TOP_RIGHT_CONNECTED},
            {false, true, true, false, std::nullopt, std::nullopt, false, std::nullopt, SoilTile::TEXTURE_TOP_LEFT_CONNECTED},
            {false, false, true, true, std::nullopt, std::nullopt, std::nullopt, std::nullopt, SoilTile::TEXTURE_LEFT_RIGHT_CONNECTED},
            {true, true, false, false, std::nullopt, std::nullopt, std::nullopt, std::nullopt, SoilTile::TEXTURE_TOP_BOTTOM_CONNECTED},
            {true, true, false, true, std::nullopt, false, std::nullopt, false, SoilTile::TEXTURE_TOP_BOTTOM_RIGHT_CONNECTED},
            {true, true, true, false, false, std::nullopt, false, std::nullopt, SoilTile::TEXTURE_TOP_BOTTOM_LEFT_CONNECTED},
            {false, true, true, true, std::nullopt, std::nullopt, false, false, SoilTile::TEXTURE_TOP_LEFT_RIGHT_CONNECTED},
            {true, false, true, true, false, false, std::nullopt, std::nullopt, SoilTile::TEXTURE_BOTTOM_LEFT_RIGHT_CONNECTED},
            {true, true, true, true, false, false, false, false, SoilTile::TEXTURE_ALL_CONNECTED},
            {true, true, true, true, true, true, true, true, SoilTile::TEXTURE_ALL_CONNECTED_ALL_CORNERS},
            {false, true, true, false, std::nullopt, std::nullopt, true, std::nullopt, SoilTile::TEXTURE_TOP_LEFT_CONNECTED_TOP_LEFT_CORNER},
            {false, true, false, true, std::nullopt, std::nullopt, std::nullopt, true, SoilTile::TEXTURE_TOP_RIGHT_CONNECTED_TOP_RIGHT_CORNER},
            {true, false, false, true, std::nullopt, true, std::nullopt, std::nullopt, SoilTile::TEXTURE_BOTTOM_RIGHT_CONNECTED_BOTTOM_RIGHT_CORNER},
            {true, false, true, false, true, std::nullopt, std::nullopt, std::nullopt, SoilTile::TEXTURE_BOTTOM_LEFT_CONNECTED_BOTTOM_LEFT_CORNER},
            {false, true, true, true, std::nullopt, std::nullopt, true, true, SoilTile::TEXTURE_TOP_LEFT_RIGHT_CONNECTED_TOP_CORNER},
            {true, true, false, true, std::nullopt, true, std::nullopt, true, SoilTile::TEXTURE_TOP_BOTTOM_RIGHT_CONNECTED_RIGHT_CORNER},
            {true, false, true, true, true, true, std::nullopt, std::nullopt, SoilTile::TEXTURE_BOTTOM_LEFT_RIGHT_CONNECTED_BOTTOM_CORNER},
            {true, true, true, false, true, std::nullopt, true, std::nullopt, SoilTile::TEXTURE_TOP_BOTTOM_LEFT_CONNECTED_LEFT_CORNER},
            {true, false, true, true, true, false, std::nullopt, std::nullopt, SoilTile::TEXTURE_BOTTOM_LEFT_RIGHT_CONNECTED_BOTTOM_LEFT_CORNER},
            {true, false, true, true, false, true, std::nullopt, std::nullopt, SoilTile::TEXTURE_BOTTOM_LEFT_RIGHT_CONNECTED_BOTTOM_RIGHT_CORNER},
            {false, true, true, true, std::nullopt, std::nullopt, false, true, SoilTile::TEXTURE_TOP_LEFT_RIGHT_CONNECTED_TOP_RIGHT_CORNER},
            {false, true, true, true, std::nullopt, std::nullopt, true, false, SoilTile::TEXTURE_TOP_LEFT_RIGHT_CONNECTED_TOP_LEFT_CORNER},
            {true, true, false, true, std::nullopt, true, std::nullopt, false, SoilTile::TEXTURE_TOP_BOTTOM_RIGHT_CONNECTED_TOP_RIGHT_CORNER},
            {true, true, false, true, std::nullopt, false, std::nullopt, true, SoilTile::TEXTURE_TOP_BOTTOM_RIGHT_CONNECTED_TOP_LEFT_CORNER},
            {true, true, true, false, true, std::nullopt, false, std::nullopt, SoilTile::TEXTURE_TOP_BOTTOM_LEFT_CONNECTED_TOP_LEFT_CORNER},
            {true, true, true, false, false, std::nullopt, true, std::nullopt, SoilTile::TEXTURE_TOP_BOTTOM_LEFT_CONNECTED_TOP_RIGHT_CORNER},
            {true, true, true, true, true, true, false, false, SoilTile::TEXTURE_ALL_CONNECTED_BOTTOM_CORNER},
            {true, true, true, true, false, true, false, true, SoilTile::TEXTURE_ALL_CONNECTED_RIGHT_CORNER},
            {true, true, true, true, false, false, true, true, SoilTile::TEXTURE_ALL_CONNECTED_TOP_CORNER},
            {true, true, true, true, true, false, true, false, SoilTile::TEXTURE_ALL_CONNECTED_LEFT_CORNER},
            {true, true, true, true, false, false, false, true, SoilTile::TEXTURE_ALL_CONNECTED_TOP_RIGHT_CORNER},
            {true, true, true, true, false, false, true, false, SoilTile::TEXTURE_ALL_CONNECTED_TOP_LEFT_CORNER},
            {true, true, true, true, true, false, false, false, SoilTile::TEXTURE_ALL_CONNECTED_BOTTOM_RIGHT_CORNER},
            {true, true, true, true, false, true, false, false, SoilTile::TEXTURE_ALL_CONNECTED_BOTTOM_LEFT_CORNER}
    };
};