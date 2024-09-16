#pragma once
#include "./include/core/rendering/SpriteManager.h"
#include "./include/core/ecs/ECS.h"
#include <vector>

// =======================================~
// You are so cool, like Hikaru Nakamura =~
// =======================================~
//
//              _______________________
//          8 /__/__/__////__////__////
//         7 /__/__////__////__////__/
//        6 /__////__////__////__////
//       5 ////__////__////__////__/
//      4 /__////__////__////__////
//     3 ////__////__////__////__/
//    2 /__////__////__////__////
//   1 ////__////__////__/__/__/
//      a  b  c  d  e  f  g  h
//
// =======================================~
// we define bishop as a piece that moves
// diagonal or as a piece that moves only
// on tiles of the same color?
// =======================================~

struct TileSet {
    std::unordered_map<char, SpriteID> tiles;
    std::unordered_map<char, Color> tileTints;
    std::unordered_map<char, bool> colliders;
};

struct TileGrid {
    float order;
    std::uint32_t width;
    std::uint32_t height;
    
    TileSet tileSet;

    std::vector<char> tiles;
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