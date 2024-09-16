#include "./include/core/rendering/TileGrid.h"
#include "./include/core/rendering/ESRendering.h"
#include "./include/core/systems/CameraControl.h"
#include "./include/core/Transform.h"

using namespace Core;

// TODO: Implement usage of tileset in tilegrid

TileGrid::TileGrid(std::uint32_t width, std::uint32_t height) 
    : width(width), height(height), order(-100) {
    tiles.reserve(width * height);
}

std::uint8_t FromChar(char c) {
    if(c >= '0' && c <= '9') return c - 48;
    if(c >= 'A' && c <= 'F') return c - 55;
    if(c >= 'a' && c <= 'f') return c - 87;
    return 0;
}

float GetRotation(char c) {
    if(c == 'v') return 90;
    if(c == '<') return 180;
    if(c == '^') return 270;
    return 0;
}

std::uint8_t FromChar(char a, char b) {
    return (FromChar(a) * 16) + FromChar(b);
}

void ReadTileSetLine(TileGrid* grid, const std::string& line) {
    bool imageRead;
    bool collider;
    char c;
    std::string data;
    SpriteID id;
    for (std::uint32_t i = 0; i < line.size(); i++) {
        if(i == 0) {
            c = line[i];
            continue;
        }

        if(i == 2) {
            collider = line[i] != '0';
            continue;
        }

        if(line[i] == ':') {
            if(i > 3) {
                id = std::stoi(data);
                imageRead = true;
            }
            data.clear();
            continue;
        }

        data.push_back(line[i]);
    }

    grid->tileSet.tiles[c] = id;
    grid->tileSet.colliders[id] = collider;
    if(data.size() == 8) {
        grid->tileSet.tileTints[id] = Color(FromChar(data[0], data[1]), FromChar(data[2], data[3]), FromChar(data[4], data[5]), FromChar(data[6], data[7]));
    } else if(data.size() == 6) {
        grid->tileSet.tileTints[id] = Color(FromChar(data[0], data[1]), FromChar(data[2], data[3]), FromChar(data[4], data[5]), 255);
    }
}

void ReadGridLine(TileGrid* grid, const std::string& line, const std::uint8_t charPerTile) {
    std::uint8_t charNum = 0;
    SpriteID sprite = 0;
    for (std::uint32_t i = 0; i < line.size(); i++) {
        if(line[i] == '-') {
            grid->tiles.push_back(-1);
            grid->rotations.push_back(0);
            i += charPerTile - charNum;
            sprite = 0;
            charNum = 0;
            continue;
        }

        charNum++;
            
        if(charNum <= charPerTile) {
            sprite *= 16;
            sprite += FromChar(line[i]);
        }

        if(charNum > charPerTile) {
            grid->tiles.push_back(sprite);
            grid->tileSet.colliders[sprite] = 0;
            grid->tileSet.tileTints[sprite] = WHITE;
            grid->rotations.push_back(GetRotation(line[i]));
            sprite = 0;
            charNum = 0;
        }
    }

    if(grid->height == 0) {
        grid->width = grid->tiles.size();
    }

    grid->height++;
}

void ReadGridLine(TileGrid* grid, const std::string& line) {
    for (std::uint32_t i = 0; i < line.size(); i += 2) {
        if(line[i] == '-') {
            grid->tiles.push_back(-1);
            grid->rotations.push_back(0);
            continue;
        }

        grid->tiles.push_back(grid->tileSet.tiles[line[i]]);
        grid->rotations.push_back(GetRotation(line[i + 1]));
    }

    if(grid->height == 0) {
        grid->width = grid->tiles.size();
    }

    grid->height++;
}

TileGrid::TileGrid(const char* fileName) : width(0), height(0), order(-10) {
    std::ifstream file(fileName);
    std::string line;

    bool readingTileSet = true;

    while (std::getline(file, line)) {
        if(line.size() > 1 && line[0] == ':' && line[1] == ':') {
            readingTileSet = false;
            continue;
        }
        if(readingTileSet) {
            ReadTileSetLine(this, line);
        } else {
            ReadGridLine(this, line);
        }
    }
    
    file.close();
}

TileGrid::TileGrid(std::uint8_t charPerTile, const char* fileName) : width(0), height(0), order(-10) {
    std::ifstream file(fileName);
    std::string line;


    while (std::getline(file, line)) {
        ReadGridLine(this, line, charPerTile);
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

            Rendering::ESRenderer::Push(grid.order, id, 
                MTransform(tr.position + (Vec2(x, y) * tr.scale.x * 2), tr.scale, grid.rotations[(grid.width * y) + x]), 
                grid.tileSet.tileTints.at(id));
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