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

void GameGrid::Init()
{
    // Open the file and read the header
    std::ifstream file(m_path, std::ios::binary);
    if(!file.is_open())
    {
        throw std::runtime_error("Failed to open file " + m_path);
    }

    auto* grid = new RawGameGrid;
    file.read(reinterpret_cast<char*>(grid), sizeof(RawGameGrid));

    // Calculate the size of the data
    uint32_t dataSize = grid->tilesetPathSize + grid->tilesSize + grid->entitiesSize;

    {
        auto* newGrid = (RawGameGrid*)malloc(sizeof(RawGameGrid) + dataSize);
        std::memcpy(newGrid, grid, sizeof(RawGameGrid));
        delete grid;
        grid = newGrid;
    }
    // Read the remaining data
    file.read(reinterpret_cast<char*>(grid->data), dataSize);

    // Reserve enough space to store all tiles
    m_tiles.reserve(grid->width * grid->height);

    // Get the tileset path (it is the first field in data)
    std::string tilesetPath(reinterpret_cast<char*>(grid->data), grid->tilesetPathSize);

    // Parse tiles
    auto* rawTile = (RawGameTile*)(grid->data + grid->tilesetPathSize);
    float x = -static_cast<float>(grid->width) / 2.0f;
    float y = -static_cast<float>(grid->height) / 2.0f;
    while(rawTile < (RawGameTile*)(grid->data + grid->tilesetPathSize + grid->tilesSize))
    {
        m_tiles.push_back(Tile::ParseRawTile(rawTile, {x, y}));
        rawTile = (RawGameTile*)((uint8_t*)rawTile + rawTile->size);
    }

    // TODO: parse entities

    m_width = grid->width;
    m_height = grid->height;
    m_tilesetTexture = Application::GetInstance().GetTextureRegistry().GetResource(tilesetPath.c_str());

    // Free the grid
    free(grid);
}

bool GameGrid::HandleEvent(const sf::Event &event)
{
    if(event.type == sf::Event::MouseWheelScrolled)
    {
        m_zoomDelta += event.mouseWheelScroll.delta;
        return true;
    }

    return false;
}

void GameGrid::Update(float deltaTime)
{
    for(auto& tile : m_tiles)
    {
        tile->Update(deltaTime, *this, m_player);
    }

    sf::Rect<float> playerBoundingBox = m_player->GetBoundingBox();
    sf::Vector2f playerPosition = m_player->GetPosition();
    float w = static_cast<float>(m_width) * TILE_SIZE / 2.0f;
    float h = static_cast<float>(m_height) * TILE_SIZE / 2.0f;

    if(playerBoundingBox.left < -w)
    {
        playerPosition.x += -w - playerBoundingBox.left;
    }
    else if(playerBoundingBox.left + playerBoundingBox.width > w)
    {
        playerPosition.x += w - playerBoundingBox.width - playerBoundingBox.left;
    }

    if(playerBoundingBox.top < -h)
    {
        playerPosition.y -= playerBoundingBox.top + h;
    }
    else if(playerBoundingBox.top + playerBoundingBox.height > h)
    {
        playerPosition.y -= playerBoundingBox.top + playerBoundingBox.height - h;
    }

    m_player->SetPosition(playerPosition);
    m_cameraPosition = playerPosition;

    m_zoomFactor += m_zoomDelta * deltaTime * ZOOM_SPEED;
    m_zoomDelta = 0;

    if(m_zoomFactor < Application::ZOOM_FACTOR + 0.5f)
    {
        m_zoomFactor = Application::ZOOM_FACTOR + 0.5f;
    }

    if(m_zoomFactor > Application::ZOOM_FACTOR + 2.0f)
    {
        m_zoomFactor = Application::ZOOM_FACTOR + 2.0f;
    }

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
        -sf::Vector2f(
            TILE_SIZE * static_cast<float>(m_width),
            TILE_SIZE * static_cast<float>(m_height)
        ) / 2.0f
    );

    window.setView({
        m_cameraPosition,
        sf::Vector2f(
            DEFAULT_VIEW_WIDTH,
            DEFAULT_VIEW_WIDTH / Application::GetInstance().GetAspectRatio()
        ) / m_zoomFactor
    });

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
            (static_cast<float>(x) * TILE_SIZE),
            (static_cast<float>(y) * TILE_SIZE)
        );

        // Calculate the texture coordinates of the tile
        // A texture coordinates (u,v) is a pair of numbers
        // that represent which part of the texture is used
        // to draw the tile
        // We do this the same way we did to calculate the
        // position of the tile in the grid
        float tu = static_cast<float>(tile->m_textureIndex % static_cast<uint64_t>(
                static_cast<float>(m_tilesetTexture->getSize().x) / TILE_SIZE));
        float tv = static_cast<float>(static_cast<float>(tile->m_textureIndex) /
                static_cast<float>(m_tilesetTexture->getSize().x) / TILE_SIZE);

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

void GameGrid::SetTile(uint32_t x, uint32_t y, std::unique_ptr<Tile> tile)
{
    m_tiles[y * m_width + x] = std::move(tile);
    m_shouldUpdateVertexArray = true;
}
