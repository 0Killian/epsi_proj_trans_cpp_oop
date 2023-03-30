//
// Created by Killian on 21/03/2023.
//

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "ResourceRegistry.h"
#include "GameObject.h"
#include "Player.h"

// The tile types the game supports (each type is associated to a class)
enum class TileType : uint8_t
{
    Ground = 0,
    Wall = 1,
    PassagePoint = 2,
    Path = 3,
    Soil = 4
};

////////////////////////////////////////////////////////////
/// \brief  The grid system of the game
///
/// This class is responsible for the grid system of the game.
/// Using factory functions, like ReadFromFile, it can load
/// a tilemap and a tileset, and use them to render a game grid
/// efficently using a vertex array (it is only built when the
/// map is modified).
/// This class is also responsible for the camera, using the
/// camera position and zoom factor, it can render the grid
/// at the right position and scale.
///
////////////////////////////////////////////////////////////
class GameGrid : public GameObject
{
public:
    GameGrid(const GameGrid& other) = delete;
    GameGrid& operator=(const GameGrid& other) = delete;

    void Init() override {};

    ////////////////////////////////////////////////////////////////////////////
    /// \brief  Updates the game grid
    ///
    /// This function updates the game grid, it checks if the vertex array needs
    /// to be rebuilt, and if so, it rebuilds it.
    ///
    /// \param deltaTime the time since the last frame
    ///
    /// \see CreateVertexArray
    /// \see GameObject::Update
    ///
    ////////////////////////////////////////////////////////////////////////////
    void Update(float deltaTime) override;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief  Renders the game grid
    ///
    /// \param window the window to render to
    ///
    /// \see GameObject::Render
    ///
    ////////////////////////////////////////////////////////////////////////////
    void Render(sf::RenderWindow& window) override;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief  Set the position of the camera
    ///
    /// \param cameraPosition the position of the camera
    ///
    ////////////////////////////////////////////////////////////////////////////
    inline void SetCameraPosition(sf::Vector2f cameraPosition) { m_cameraPosition = cameraPosition; }

    ////////////////////////////////////////////////////////////////////////////
    /// \brief  Set the zoom factor of the camera
    ///
    /// \param zoomFactor the zoom factor of the camera
    ///
    ////////////////////////////////////////////////////////////////////////////
    inline void SetZoomFactor(float zoomFactor) override { m_zoomFactor = zoomFactor; }

    ////////////////////////////////////////////////////////////////////////////
    /// \brief  Set the player
    ///
    /// \param player the player
    ///
    /// \see Player
    ////////////////////////////////////////////////////////////////////////////
    inline void SetPlayer(const std::shared_ptr<Player>& player) { m_player = player; }

    ////////////////////////////////////////////////////////////////////////////
    /// \brief  A factory function to create a game grid from a file
    ///
    /// This function needs a path to a file in the HTF format described here :
    ///
    /// Header:
    /// -----------------------------------------------------------------------------------------------------------------
    /// | width    | height   | tilesetPathSize | tilesSize | entitiesSize | tilesetPath     | tiles     | entities     |
    /// -----------------------------------------------------------------------------------------------------------------
    /// | uint32_t | uint32_t | uint32_t        | uint32_t  | uint32_t     | char[]          | Tile[]    | Entity[]     |
    /// -----------------------------------------------------------------------------------------------------------------
    /// | 4        | 4        | 4               | 4         | 4            | tileSetPathSize | tilesSize | entitiesSize |
    /// -----------------------------------------------------------------------------------------------------------------
    ///
    /// Tile:
    /// --------------------------------------------------
    /// | type     | size     | textureIndex | data      |
    /// --------------------------------------------------
    /// | uint8_t  | uint32_t | uint32_t     | uint8_t[] |
    /// --------------------------------------------------
    /// | 1        | 4        | 4            | size - 9  |
    /// --------------------------------------------------
    ///
    /// Entity: TODO
    /// \param path the path to the file
    /// \return a new game grid
    ///
    ////////////////////////////////////////////////////////////////////////////
    static std::unique_ptr<GameGrid> ReadFromFile(const std::string& path);

private:
// We need to pack the structures to tell to the compiler to not add any padding
#pragma pack(push, 1)
    // The header of the file
    struct RawGameGrid
    {
        uint32_t width;
        uint32_t height;
        uint32_t tilesetPathSize; // size of the tilemaps path
        uint32_t tilesSize; // total size of tiles
        uint32_t entitiesSize; // total size of entities
        uint8_t data[];
    };

    // The tile structure in the file
    struct RawGameTile
    {
        TileType type;
        uint32_t size; // size of total structure
        uint32_t textureIndex; // offset in texture names
        uint8_t data[]; // custom data
    };

    // The entity structure in the file (TODO)
    struct RawGameEntity
    {
        uint32_t size; // size of total structure
        uint32_t textureIndex; // offset in texture names
        uint8_t data[]; // custom data
    };
#pragma pack(pop)

public:
    ////////////////////////////////////////////////////////////////////////////
    /// \brief  A class representing a tile
    ///
    /// This class is the base class for all tiles. It is responsible for
    /// the texture index of the tile, and the parsing of the raw tile
    /// structure.
    ///
    /// \see Tile::ParseRawTile
    ////////////////////////////////////////////////////////////////////////////
    class Tile
    {
    public:
        explicit Tile(uint64_t textureIndex) : m_textureIndex(textureIndex) {}
        Tile(const Tile& other) = delete;
        Tile& operator=(const Tile& other) = delete;

        virtual ~Tile() = default;

        virtual void Update(float deltaTime, GameGrid& grid, const std::shared_ptr<Player>& player) = 0;

    protected:
        friend GameGrid;

        ////////////////////////////////////////////////////////////////////////////
        /// \brief  A factory function to create a tile from a the raw tile structure
        ///
        /// \param rawTile the raw tile structure
        /// \return A new tile subclass
        ///
        ////////////////////////////////////////////////////////////////////////////
        static std::unique_ptr<Tile> ParseRawTile(RawGameTile* rawTile);

    private:
        uint64_t m_textureIndex;
    };

    static constexpr float TILE_SIZE = 32.0f;

private:
    ////////////////////////////////////////////////////////////////////////////
    /// \brief  Creates a game grid from the tilemap and the tileset
    ///
    /// \param tiles the tilemap of the grid
    /// \param tileset the tileset used by the tilemap
    /// \param width the width of the grid
    /// \param height the height of the grid
    ///
    ////////////////////////////////////////////////////////////////////////////
    explicit GameGrid(std::vector<std::unique_ptr<Tile>>&& tiles, const std::string& tileset, int width, int height);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief  Creates a vertex array from the tilemap
    ///
    /// This function needs to be called when the tilemap is modified, it will
    /// rebuild the vertex array.
    ///
    /// \return a vertex array
    ///
    ////////////////////////////////////////////////////////////////////////////
    sf::VertexArray CreateVertexArray();


    std::vector<std::unique_ptr<Tile>> m_tiles;

    TextureRegistry::ResourceHandle m_tilesetTexture;

    sf::VertexArray m_vertexArray;
    std::atomic_bool m_shouldUpdateVertexArray = true;
    std::mutex m_vertexArrayMutex;

    uint32_t m_width;
    uint32_t m_height;

    sf::Vector2f m_cameraPosition = {0, 0};
    float m_zoomFactor = 1.0f;

    std::shared_ptr<Player> m_player;
};