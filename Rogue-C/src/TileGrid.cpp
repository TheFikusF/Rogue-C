#include "./include/core/rendering/TileGrid.h"
#include "./include/core/rendering/ESRendering.h"
#include "./include/core/systems/CameraControl.h"
#include "./include/core/Transform.h"

using namespace Core;


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
    grid->tileSet.colliders[c] = collider;
    if(data.size() == 8) {
        grid->tileSet.tileTints[c] = Color(FromChar(data[0], data[1]), FromChar(data[2], data[3]), FromChar(data[4], data[5]), FromChar(data[6], data[7]));
    } else if(data.size() == 6) {
        grid->tileSet.tileTints[c] = Color(FromChar(data[0], data[1]), FromChar(data[2], data[3]), FromChar(data[4], data[5]), 255);
    }
}

void ReadGridLine(TileGrid* grid, const std::string& line, const std::uint8_t charPerTile, std::unordered_map<SpriteID, char>& spriteToChar, char& maxChar) {
    std::uint8_t charNum = 0;
    SpriteID sprite = 0;
    for (std::uint32_t i = 0; i < line.size(); i++) {
        if(line[i] == '-') {
            grid->tiles.push_back('-');
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
            if(spriteToChar.find(sprite) == spriteToChar.end()) {
                spriteToChar[sprite] = ++maxChar == '-' ? ++maxChar : maxChar;
            }

            grid->tiles.push_back(spriteToChar.at(sprite));
            grid->rotations.push_back(GetRotation(line[i]));
            
            grid->tileSet.tiles[spriteToChar.at(sprite)] = sprite;
            grid->tileSet.colliders[spriteToChar.at(sprite)] = 0;
            grid->tileSet.tileTints[spriteToChar.at(sprite)] = WHITE;
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

        grid->tiles.push_back(line[i]);
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

    std::unordered_map<SpriteID, char> spriteToChar;
    char maxChar = 0;

    while (std::getline(file, line)) {
        ReadGridLine(this, line, charPerTile, spriteToChar, maxChar);
    }
    
    file.close();
}

// TODO: implement tilegrid collider

TileGridSystem::TileGridSystem() {
    signature.set(Core::ECS::GetComponentType<TileGrid>());
    signature.set(Core::ECS::GetComponentType<MTransform>());
}

void RenderGrid(const TileGrid& grid, const MTransform& tr) {
    //float width = grid.size.x * tr.scale.x;
    for(std::uint32_t y = 0; y < grid.height; y++) {
        for(std::uint32_t x = 0; x < grid.width; x++) {
            char id = grid.tiles[(grid.width * y) + x];
            if(id == '-') {
                continue;
            }

            Rendering::ESRenderer::Push(grid.order, grid.tileSet.tiles.at(id), 
                MTransform(tr.position + (Vec2(x, y) * tr.scale.x * 2), tr.scale, grid.rotations[(grid.width * y) + x]), 
                grid.tileSet.tileTints.at(id));
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