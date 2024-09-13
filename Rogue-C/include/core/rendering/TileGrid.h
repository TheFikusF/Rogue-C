#pragma once
#include "./include/core/rendering/SpriteManager.h"
#include "./include/core/ecs/ECS.h"
#include <vector>

struct TileSet {
    std::vector<SpriteID> tiles;
};

struct TileGrid {
    std::uint32_t width;
    std::uint32_t height;
    TileSet* tileSet;
    std::vector<SpriteID> tiles;

    TileGrid() : tileSet(nullptr) { }
    TileGrid(std::uint32_t width, std::uint32_t height);
    TileGrid(std::uint8_t charPerTile, const char* filePath);
};

class TileGridSystem : public Core::System {
public:
    TileGridSystem();

    void Draw() override;
};