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
    static void Init() {
        Image image = GenImageColor(10, 10, WHITE);
        textures.emplace_back(LoadTextureFromImage(image));
    }

    static Sprite RegisterTexture(const char* path) { 
        Image image = LoadImage(path);
        textures.emplace_back(LoadTextureFromImage(image));
        return textures.size() - 1; 
    }

    static Texture2D& GetTexture(const Sprite& sprite) {
        return textures[sprite];
    }

    static void UnloadAll() {
        for (int i = textures.size() - 1; i >= 0; i--) {
            UnloadTexture(textures[i]);
        }
    }
};