#pragma once
#include <vector>
#include "./include/raylib/raylib.h"
#include <cstdint>

using SpriteID = std::uint32_t;
using TextureID = std::uint32_t;
using ShaderID = std::uint32_t;
constexpr ShaderID DEFAULT_SHADER = 0;

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
    std::vector<Shader> shaders;
    
public:
    static void Init();

    static TextureID RegisterTexture(const char* path);
    static ShaderID RegisterShader(const char* path);
    static SpriteID RegisterSprite(const TextureID texture);
    static SpriteID RegisterSprite(const TextureID texture, Rectangle rect);

    static Texture2D& GetTexture(const TextureID& texture);
    static Sprite& GetSprite(const SpriteID& sprite);
    static Shader& GetShader(const ShaderID& shader);

    static void UnloadAll();
};