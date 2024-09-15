#include "./include/core/rendering/TileGrid.h"
#include "./include/core/rendering/ESRendering.h"
#include "./include/core/systems/CameraControl.h"
#include "./include/core/Transform.h"

using namespace Core;

// TODO: Implement usage of tileset in tilegrid

TileGrid::TileGrid(std::uint32_t width, std::uint32_t height) 
    : tileSet(nullptr), width(width), height(height), order(-100) {
    tiles.reserve(width * height);
}

std::uint8_t FromChar(char c) {
    if(c >= '0' && c <= '9') return c - 48;
    if(c >= 'A' && c <= 'F') return c - 55;
    if(c >= 'a' && c <= 'f') return c - 87;
    return 0;
}

TileGrid::TileGrid(std::uint8_t charPerTile, const char* fileName) : tileSet(nullptr), width(0), height(0) {
    std::ifstream file(fileName);
    std::string line;

    while (std::getline(file, line)) {
        std::uint8_t charNum = 0;
        SpriteID sprite = 0;
        for (std::uint32_t i = 0; i < line.size(); i++) {
            if(line[i] == '-') {
                tiles.push_back(-1);
                i += charPerTile - charNum - 1;
                sprite = 0;
                charNum = 0;
                continue;
            }
            charNum++;
            sprite *= 16;
            sprite += FromChar(line[i]);
            if(charNum == charPerTile) {
                tiles.push_back(sprite);
                sprite = 0;
                charNum = 0;
            }
        }

        if(height == 0) {
            width = tiles.size();
        }

        height++;
    }
    
    file.close();
}

// TODO: I NEED TO LIKE MAKE A DOUBLE BUFFER WITH SORTED HASH_MAP I 
// DUNO AND ALL RENDERERS ARE SUBMITIG TO IT WHAT AND AT WHAT ORDER 
// THEY WANT TO RENDER THE THING AND THEN IT JUST YA KNOW WHAT? 
// TAKES AND RENDERS IT ON THE NEXT FRAME LOLLLLLLLL

// TODO: implement tilegrid collider

TileGridSystem::TileGridSystem() {
    signature.set(Core::ECS::GetComponentType<TileGrid>());
    signature.set(Core::ECS::GetComponentType<MTransform>());
}

void RenderGrid(const TileGrid& grid, const MTransform& tr) {
    //float width = grid.size.x * tr.scale.x;
    for(std::uint32_t y = 0; y < grid.height; y++) {
        for(std::uint32_t x = 0; x < grid.width; x++) {
            SpriteID id = grid.tiles[(grid.width * y) + x];
            if(id == -1) {
                continue;
            }

            const Sprite& sprite = SpriteManager::GetSprite(id);
            const Texture2D& tex = SpriteManager::GetTexture(sprite.texture);

            Rendering::ESRenderer::Push(grid.order, id, MTransform(tr.position + (Vec2(x, y) * tr.scale.x * 2), tr.scale, 0), WHITE);
            // DrawTexturePro(tex, sprite.rect, 
            //     { tr.position.x + ((float)x * tr.scale.x * 2), tr.position.y + ((float)y * tr.scale.y * 2), tr.scale.x * 2.0f, tr.scale.y * 2.0f },
            //     { 0, 0 }, 0,  WHITE);
        }
    }
}

void TileGridSystem::Draw() {
    BeginMode2D(CameraContorl::GetCurrent());
    for(Entity entity : Entities) {
        RenderGrid(ECS::GetComponent<TileGrid>(entity), ECS::GetComponent<MTransform>(entity));
    }
    EndMode2D();
}