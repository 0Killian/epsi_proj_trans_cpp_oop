#include <cstdint>
#include <string>
#include <fstream>

enum class TileType : uint8_t
{
    Ground = 0,
    Wall = 1,
    PassagePoint = 2,
    Path = 3,
    Soil = 4
};

#pragma pack(push, 1)
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

class Tile
{
public:
    explicit Tile(uint64_t textureIndex) : m_textureIndex(textureIndex) {}
    virtual ~Tile() = default;

    virtual RawGameTile* Serialize() = 0;

    uint64_t m_textureIndex;
};


class GroundTile : public Tile
{
public:
    explicit GroundTile(uint64_t textureIndex) : Tile(textureIndex) {}

    RawGameTile* Serialize() override
    {
        RawGameTile* tile = new RawGameTile();
        tile->type = TileType::Ground;
        tile->size = sizeof(RawGameTile);
        tile->textureIndex = m_textureIndex;
        return tile;
    }
};

class WallTile : public Tile
{
public:
    explicit WallTile(uint64_t textureIndex) : Tile(textureIndex) {}

    RawGameTile* Serialize() override
    {
        RawGameTile* tile = new RawGameTile();
        tile->type = TileType::Wall;
        tile->size = sizeof(RawGameTile);
        tile->textureIndex = m_textureIndex;
        return tile;
    }
};

class PassagePointTile : public Tile
{
public:
    PassagePointTile(uint64_t textureIndex, std::string tilemap, std::pair<uint32_t, uint32_t> position)
        : Tile(textureIndex), m_tilemap(tilemap), m_position(position) {}

    RawGameTile* Serialize() override
    {
        uint64_t size = sizeof(RawGameTile) + m_tilemap.size() + sizeof(uint64_t) * 3;
        RawGameTile* tile = (RawGameTile*)malloc(size);
        tile->type = TileType::PassagePoint;
        tile->size = size;
        tile->textureIndex = m_textureIndex;
        tile->data[0] = m_tilemap.size();
        memcpy(tile->data + 1, m_tilemap.c_str(), m_tilemap.size());
        tile->data[m_tilemap.size() + 1] = m_position.first;
        tile->data[m_tilemap.size() + 2] = m_position.second;
        return tile;
    }

private:
    std::string m_tilemap;
    std::pair<uint64_t, uint64_t> m_position;
};

class PathTile : public Tile
{
public:
    explicit PathTile(uint64_t textureIndex) : Tile(textureIndex) {}

    RawGameTile* Serialize() override
    {
        RawGameTile* tile = new RawGameTile();
        tile->type = TileType::Path;
        tile->size = sizeof(RawGameTile);
        tile->textureIndex = m_textureIndex;
        return tile;
    }
};

class SoilTile : public Tile
{
public:
    // TODO: support plants
    SoilTile(uint64_t textureIndex) : Tile(textureIndex) {}

    RawGameTile* Serialize() override
    {
        RawGameTile* tile = new RawGameTile();
        tile->type = TileType::Soil;
        tile->size = sizeof(RawGameTile);
        tile->textureIndex = m_textureIndex;
        return tile;
    }
};

std::pair<uint64_t, uint8_t*> SerializeTilemap(std::string tilesetPath, Tile* tilemap[], uint32_t width, uint32_t height)
{
    uint32_t tilesetPathSize = tilesetPath.size();
    uint32_t tilesSize = 0;
    uint32_t entitiesSize = 0;
    for(uint32_t i = 0; i < width * height; i++)
    {
        tilesSize += tilemap[i]->Serialize()->size;
    }

    uint32_t size = sizeof(RawGameGrid) + tilesetPathSize + tilesSize + entitiesSize;
    RawGameGrid* grid = (RawGameGrid*)malloc(size);
    grid->width = width;
    grid->height = height;
    grid->tilesetPathSize = tilesetPathSize;
    grid->tilesSize = tilesSize;
    grid->entitiesSize = entitiesSize;
    memcpy(grid->data, tilesetPath.c_str(), tilesetPathSize);
    uint32_t offset = tilesetPathSize;
    for(uint32_t i = 0; i < width * height; i++)
    {
        RawGameTile* tile = tilemap[i]->Serialize();
        memcpy(grid->data + offset, tile, tile->size);
        offset += tile->size;
        delete tile;
    }

    return {size, (uint8_t*)grid};
}

int main(int argc, char** argv)
{
    std::string outPath = "tilemap.htf";
    if(argc > 1)
    {
        outPath = argv[1];
    }

    srand(time(NULL));
    Tile* tilemap[50*50];
    for(Tile*& tile : tilemap)
    {
        uint64_t random = rand() % 4;
        if(random == 3) random = 9;
        tile = new GroundTile(random);
    }

    auto [size, data] = SerializeTilemap("tileset.png", tilemap, 50, 50);
    std::ofstream file(outPath, std::ios::binary);
    file.write((char*)data, size);
}
