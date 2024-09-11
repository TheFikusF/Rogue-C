#pragma once
#include "./include/core/rendering/SpriteManager.h"
#include "./include/core/ecs/ECS.h"
#include <vector>

struct TileSet {
    std::vector<SpriteID> tiles;
};

struct TileGrid {
    std::uint16_t width;
    std::uint16_t height;
    TileSet* tileSet;
    std::vector<SpriteID> tiles;

    TileGrid() : tileSet(nullptr) { }
    TileGrid(std::uint16_t width, std::uint16_t height) : tileSet(nullptr), width(width), height(height) { tiles.reserve(width * height); }
};

class TileGridSystem : public Core::System {
    TileGridSystem();

    void Draw() override;
};