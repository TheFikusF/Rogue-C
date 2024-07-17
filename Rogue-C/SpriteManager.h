#pragma once
#include <vector>
#include "./include/raylib/raylib.h"
#include <cstdint>

using Sprite = std::size_t;

class SpriteManager
{
private:
    static std::vector<Texture2D> textures;
    
public:
    static Sprite RegisterTexture(const char* path) { 
        textures.emplace_back(LoadTexture(path));
        return textures.size() - 1; 
    }

    static Texture2D& GetTexture(const Sprite& sprite) {
        return textures[sprite];
    }
};