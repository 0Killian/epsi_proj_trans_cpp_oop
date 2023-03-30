//
// Created by Killian on 21/03/2023.
//
#include <GameGrid.h>
#include <fstream>
#include <Application.h>
#include <Tiles.h>

std::unique_ptr<GameGrid::Tile> GameGrid::Tile::ParseRawTile(RawGameTile* rawTile, sf::Vector2f position)
{
    // Check against all available types, and return nullptr if none match
    switch(rawTile->type)
    {
    case TileType::Ground:
        return std::make_unique<GroundTile>(rawTile->textureIndex, position);
    case TileType::Wall:
        return std::make_unique<WallTile>(rawTile->textureIndex, position);
    case TileType::PassagePoint:
        return std::make_unique<PassagePointTile>(rawTile->textureIndex, position, rawTile->data, rawTile->size);
    case TileType::Path:
        return std::make_unique<PathTile>(rawTile->textureIndex, position);
    case TileType::Soil:
        return std::make_unique<SoilTile>(rawTile->textureIndex, position, rawTile->data, rawTile->size);
    default:
        return nullptr;
    }
}

std::shared_ptr<GameGrid> GameGrid::ReadFromFile(const std::string &path)
{
    std::vector<std::unique_ptr<Tile>> tiles;

    // Open the file and read the header
    std::ifstream file(path, std::ios::binary);
    if(!file.is_open())
    {
        return nullptr;
    }

    auto* grid = new RawGameGrid;
    file.read(reinterpret_cast<char*>(grid), sizeof(RawGameGrid));

    // Calculate the size of the data
    uint32_t dataSize = grid->tilesetPathSize + grid->tilesSize + grid->entitiesSize;

    {
        auto* newGrid = (RawGameGrid*)malloc(sizeof(RawGameGrid) + dataSize);
        std::memcpy(newGrid, grid, sizeof(RawGameGrid));
        grid = newGrid;
    }
    // Read the remaining data
    file.read(reinterpret_cast<char*>(grid->data), dataSize);

    // Reserve enough space to store all tiles
    tiles.reserve(grid->width * grid->height);

    // Get the tileset path (it is the first field in data)
    std::string tilesetPath(reinterpret_cast<char*>(grid->data), grid->tilesetPathSize);

    // Parse tiles
    auto* rawTile = (RawGameTile*)(grid->data + grid->tilesetPathSize);
    float x = -grid->width / 2;
    float y = -grid->height / 2;
    while(rawTile < (RawGameTile*)(grid->data + grid->tilesetPathSize + grid->tilesSize))
    {
        tiles.push_back(Tile::ParseRawTile(rawTile, {x, y}));
        rawTile = (RawGameTile*)((uint8_t*)rawTile + rawTile->size);
    }

    // TODO: parse entities

    return std::shared_ptr<GameGrid>(new GameGrid(std::move(tiles), tilesetPath, grid->width, grid->height));

}

GameGrid::GameGrid(std::vector<std::unique_ptr<Tile>>&& tiles, const std::string& tileset, int width, int height)
    : m_width(width), m_height(height), m_tiles(std::move(tiles))
{
    m_tilesetTexture = Application::GetInstance().GetTextureRegistry().GetResource(tileset.c_str());
}

void GameGrid::Update(float deltaTime)
{
    for(auto& tile : m_tiles)
    {
        tile->Update(deltaTime, *this, m_player);
    }

    sf::Rect<float> playerBoundingBox = m_player->GetBoundingBox();
    sf::Vector2f playerPosition = m_player->GetPosition();
    if(playerBoundingBox.left < -static_cast<float>(m_width) * TILE_SIZE / 2)
    {
        playerPosition.x += -static_cast<float>(m_width) * TILE_SIZE / 2 - playerBoundingBox.left;
    }
    else if(playerBoundingBox.left + playerBoundingBox.width > static_cast<float>(m_width) * TILE_SIZE / 2)
    {
        playerPosition.x += static_cast<float>(m_width) * TILE_SIZE / 2 - playerBoundingBox.width - playerBoundingBox.left;
    }

    if(playerBoundingBox.top < -static_cast<float>(m_height) * TILE_SIZE / 2)
    {
        playerPosition.y -= playerBoundingBox.top + static_cast<float>(m_height) * TILE_SIZE / 2;
    }
    else if(playerBoundingBox.top + playerBoundingBox.height > static_cast<float>(m_height) * TILE_SIZE / 2)
    {
        playerPosition.y -= playerBoundingBox.top + playerBoundingBox.height - static_cast<float>(m_height) * TILE_SIZE / 2;
    }

    m_player->SetPosition(playerPosition);
    m_cameraPosition = playerPosition;

    if(m_shouldUpdateVertexArray)
    {
        // Update the vertex array
        m_vertexArrayMutex.lock();
        m_vertexArray = std::move(CreateVertexArray());
        m_vertexArrayMutex.unlock();
        m_shouldUpdateVertexArray = false;
    }
}

void GameGrid::Render(sf::RenderWindow& window)
{
    sf::RenderStates states;
    states.texture = m_tilesetTexture.GetPointer();

    // Calculate the transform matrix for the grid
    // It is dependent on the camera position and the size of the window (to center the grid)
    states.transform.translate(
        -m_cameraPosition * (Application::ZOOM_FACTOR + m_zoomFactor) +
        sf::Vector2f(
                Application::WINDOW_WIDTH - TILE_SIZE * static_cast<float>(m_width) * (Application::ZOOM_FACTOR + m_zoomFactor),
                Application::WINDOW_HEIGHT - TILE_SIZE * static_cast<float>(m_height) * (Application::ZOOM_FACTOR + m_zoomFactor)
        ) / 2.0f
    );

    states.transform.scale({Application::ZOOM_FACTOR + m_zoomFactor, Application::ZOOM_FACTOR + m_zoomFactor});


    window.draw(m_vertexArray, states);
}

sf::VertexArray GameGrid::CreateVertexArray()
{
    // A vertex array is a list of structures called vertices.
    // Each vertex contains a position, a color and a texture coordinate
    // The renderer uses these vertices to draw a primitive
    // (a triangle, a line, a point, etc.)
    // Here we create a vertex array that will contain vertices
    // defining triangles
    sf::VertexArray vertexArray(sf::PrimitiveType::Triangles);

    for(uint64_t i = 0; i < m_tiles.size(); i++)
    {
        std::unique_ptr<Tile>& tile = m_tiles[i];

        // Calculate the position of the tile in the grid
        uint64_t x = i % m_width;
        uint64_t y = i / m_width;

        // Calculate its position in the vertex array
        // We do not apply any transformations here (like the camera position)
        // because the vertex array will be transformed by the renderer
        // in the render function using RenderStates
        sf::Vector2f position(
            (static_cast<float>(x) * TILE_SIZE - m_cameraPosition.x),
            (static_cast<float>(y) * TILE_SIZE - m_cameraPosition.y)
        );

        // Calculate the texture coordinates of the tile
        // A texture coordinates (u,v) is a pair of numbers
        // that represent which part of the texture is used
        // to draw the tile
        // We do this the same way we did to calculate the
        // position of the tile in the grid
        float tu = static_cast<float>(tile->m_textureIndex % static_cast<uint64_t>(
                static_cast<float>(m_tilesetTexture->getSize().x) / TILE_SIZE));
        float tv = static_cast<float>(tile->m_textureIndex / static_cast<uint64_t>(
                static_cast<float>(m_tilesetTexture->getSize().x) / TILE_SIZE));

        sf::IntRect textureRect(
            {static_cast<int>(tu * TILE_SIZE), static_cast<int>(tv * TILE_SIZE)},
            {static_cast<int>(TILE_SIZE), static_cast<int>(TILE_SIZE)}
        );

        // Add the vertices to the vertex array
        // We need to add draw 2 triangles in a square
        // to draw the tile, so we add 6 vertices
        vertexArray.append(sf::Vertex(
            position,
            {static_cast<float>(textureRect.left), static_cast<float>(textureRect.top)}
        ));
        vertexArray.append(sf::Vertex(
            position + sf::Vector2f(TILE_SIZE, 0),
            {static_cast<float>(textureRect.left + textureRect.width), static_cast<float>(textureRect.top)}
        ));
        vertexArray.append(sf::Vertex(
            position + sf::Vector2f(0, TILE_SIZE),
            {static_cast<float>(textureRect.left), static_cast<float>(textureRect.top + textureRect.height)}
        ));

        vertexArray.append(sf::Vertex(
            position + sf::Vector2f(TILE_SIZE, 0),
            {static_cast<float>(textureRect.left + textureRect.width), static_cast<float>(textureRect.top)}
        ));
        vertexArray.append(sf::Vertex(
            position + sf::Vector2f(TILE_SIZE, TILE_SIZE),
            {static_cast<float>(textureRect.left + textureRect.width), static_cast<float>(textureRect.top + textureRect.height)}
        ));
        vertexArray.append(sf::Vertex(
            position + sf::Vector2f(0, TILE_SIZE),
            {static_cast<float>(textureRect.left), static_cast<float>(textureRect.top + textureRect.height)}
        ));
    }

    return vertexArray;
}


