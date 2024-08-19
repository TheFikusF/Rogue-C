#include "SpriteManager.h"

SpriteManager* instance;

Sprite::Sprite(TextureID texture, Rectangle rect) : texture(texture), rect(rect) {}

Sprite::Sprite(TextureID texture)
    : texture(texture), rect{0, 0, SpriteManager::GetTexture(texture).width, SpriteManager::GetTexture(texture).height} {}

void SpriteManager::Init() {
    instance = new SpriteManager();
    Image image1 = GenImageColor(10, 10, WHITE);
    Image image2 = GenImageColor(64, 64, Color(0, 0, 0, 0));
    ImageDrawCircle(&image2, 32, 32, 32, WHITE);
    instance->textures.emplace_back(LoadTextureFromImage(image1));
    instance->textures.emplace_back(LoadTextureFromImage(image2));
    RegisterSprite(0);
    RegisterSprite(1);

    instance->shaders.push_back(Shader());
}

SpriteID SpriteManager::RegisterTexture(const char* path) {
    Image image = LoadImage(path);
    instance->textures.emplace_back(LoadTextureFromImage(image));
    return instance->textures.size() - 1;
}

ShaderID SpriteManager::RegisterShader(const char* path) { 
    instance->shaders.emplace_back(LoadShader(0, path));
    return instance->shaders.size() - 1;
}

SpriteID SpriteManager::RegisterSprite(const TextureID texture) {
    return RegisterSprite(texture, Rectangle{ .x = 0, .y = 0, .width = float(GetTexture(texture).width), .height = float(GetTexture(texture).height) });
}

SpriteID SpriteManager::RegisterSprite(const TextureID texture, Rectangle rect) {
    instance->sprites.emplace_back(texture, rect);
    return instance->sprites.size() - 1;
}

Texture2D& SpriteManager::GetTexture(const TextureID& texture) {
    return instance->textures[texture];
}

Sprite& SpriteManager::GetSprite(const SpriteID& sprite) {
    return instance->sprites[sprite];
}

Shader& SpriteManager::GetShader(const ShaderID& shader) {
    return instance->shaders[shader];
}

void SpriteManager::UnloadAll() {
    for(auto& texture : instance->textures) {
        UnloadTexture(texture);
    }

    for(auto& shader : instance->shaders) {
        UnloadShader(shader);
    }
}