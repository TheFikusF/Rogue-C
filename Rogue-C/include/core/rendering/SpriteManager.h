#pragma once
#include "./include/raylib/raylib.h"
#include <unordered_map>
#include <string>
#include <vector>
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
    std::unordered_map<std::string, TextureID> namesMap;
    std::vector<Sprite> sprites;
    std::vector<Texture2D> textures;
    std::vector<Shader> shaders;
    
public:
    static void Init();
    static void LoadTexturesFromText(const char* path);

    static TextureID RegisterTexture(const char* path);
    static ShaderID RegisterShader(const char* path);
    static SpriteID RegisterSprite(const TextureID texture);
    static SpriteID RegisterSprite(const TextureID texture, Rectangle rect);

    static Texture2D& GetTexture(const TextureID& texture);
    static Texture2D& GetTexture(const char* name);
    static Sprite& GetSprite(const SpriteID& sprite);
    static Shader& GetShader(const ShaderID& shader);

    static void UnloadAll();
};