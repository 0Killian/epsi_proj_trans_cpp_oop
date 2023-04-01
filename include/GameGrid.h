//
// Created by Killian on 21/03/2023.
//

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
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

    ////////////////////////////////////////////////////////////////////////////
    /// \brief  The constructor
    ///
    /// The file needs to be in the HTF format described here :
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
    ///
    /// \note This does not initializes the game grid, you need to call Init
    /// \see Init
    ///
    /// \param path the path to the file
    ///
    ////////////////////////////////////////////////////////////////////////////
    explicit GameGrid(std::string path) : m_path(std::move(path)) {}

    ////////////////////////////////////////////////////////////////////////////
    /// \brief  Initializes the game grid using the path given in the constructor
    ///
    /// This function initializes the game grid, it loads the tileset and the
    /// tilemap, and creates the vertex array.
    ///
    /// \see CreateVertexArray
    ///
    ////////////////////////////////////////////////////////////////////////////
    void Init() override;

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
    /// \brief  Handles the wheel scroll event
    ///
    /// The game grid is responsible for the camera, so it handles the wheel
    /// scroll event to zoom in and out.
    ///
    /// \param event the event to handle
    /// \return true if other objects should not handle the event
    ///
    /// \see GameObject::HandleEvent
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool HandleEvent(const sf::Event& event) override;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief  Set the player
    ///
    /// \param player the player
    ///
    /// \see Player
    ////////////////////////////////////////////////////////////////////////////
    inline void SetPlayer(const std::shared_ptr<Player>& player) { m_player = player; }

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
        explicit Tile(uint64_t textureIndex, sf::Vector2f position) : m_textureIndex(textureIndex), m_position(position) {}
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
        static std::unique_ptr<Tile> ParseRawTile(RawGameTile* rawTile, sf::Vector2f position);

        sf::Vector2f m_position;

    private:
        uint64_t m_textureIndex;
    };

    ////////////////////////////////////////////////////////////////////////////
    /// \brief  Set the tile at the given position
    ///
    /// \param x the x position
    /// \param y the y position
    /// \param tile the tile to set
    ///
    /// \see Tile
    ////////////////////////////////////////////////////////////////////////////
    void SetTile(uint32_t x, uint32_t y, std::unique_ptr<Tile> tile);

    static constexpr float TILE_SIZE = 32.0f;

protected:
    friend Tile;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief  Triggers the vertex array update
    ///
    /// This function triggers the vertex array update, it will be updated on the
    /// next frame.
    ///
    /// \see CreateVertexArray
    ///
    ////////////////////////////////////////////////////////////////////////////
    inline void TriggerVertexArrayUpdate() { m_shouldUpdateVertexArray = true; }

private:
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

    static constexpr float ZOOM_SPEED = 20.0f;
    static constexpr float DEFAULT_VIEW_WIDTH = 39.0f * TILE_SIZE;

    std::string m_path;

    TextureRegistry::ResourceHandle m_tilesetTexture;
    std::vector<std::unique_ptr<Tile>> m_tiles;
    uint32_t m_width = 0;
    uint32_t m_height = 0;

    sf::VertexArray m_vertexArray;
    std::atomic_bool m_shouldUpdateVertexArray = true;
    std::mutex m_vertexArrayMutex;

    sf::Vector2f m_cameraPosition = {0, 0};
    float m_zoomFactor = 1.0f;
    float m_zoomDelta = 0.0f;

    std::shared_ptr<Player> m_player;
};