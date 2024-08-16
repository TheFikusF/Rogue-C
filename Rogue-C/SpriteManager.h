#pragma once
#include <vector>
#include "./include/raylib/raylib.h"
#include <cstdint>

using SpriteID = std::uint32_t;
using TextureID = std::uint32_t;

struct Sprite {
    TextureID texture;
    Rectangle rect;

    Sprite(TextureID texture, Rectangle rect);
    Sprite(TextureID texture);
};

class SpriteManager
{
private:
    std::vector<Sprite> sprites;
    std::vector<Texture2D> textures;
    
public:
    static void Init();

    static TextureID RegisterTexture(const char* path);
    static SpriteID RegisterSprite(const TextureID texture);
    static SpriteID RegisterSprite(const TextureID texture, Rectangle rect);

    static Texture2D& GetTexture(const TextureID& sprite);
    static Sprite& GetSprite(const SpriteID& sprite);

    static void UnloadAll();
};