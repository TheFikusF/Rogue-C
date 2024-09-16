#pragma once
#include "./include/core/rendering/SpriteManager.h"
#include "./include/core/ecs/ECS.h"
#include <vector>

struct TileSet {
    std::unordered_map<char, SpriteID> tiles;
    std::unordered_map<SpriteID, Color> tileTints;
    std::unordered_map<SpriteID, bool> colliders;
};

struct TileGrid {
    float order;
    std::uint32_t width;
    std::uint32_t height;
    
    TileSet tileSet;

    std::vector<SpriteID> tiles;
    std::vector<float> rotations;

    TileGrid() { }
    TileGrid(std::uint32_t width, std::uint32_t height);
    TileGrid(const char* filePath);
    TileGrid(std::uint8_t charPerTile, const char* filePath);
};

class TileGridSystem : public Core::System {
public:
    TileGridSystem();

    void Draw() override;
};