#include "./include/core/rendering/SpriteManager.h"
#include <fstream>
#include <iostream>

SpriteManager* instance;

Sprite::Sprite(TextureID texture, Rectangle rect) : texture(texture), rect(rect) {}

Sprite::Sprite(TextureID texture)
    : texture(texture), rect{0, 0, (float)SpriteManager::GetTexture(texture).width, (float)SpriteManager::GetTexture(texture).height} {}

void SpriteManager::Init() {
    instance = new SpriteManager();
    Image image1 = GenImageColor(10, 10, WHITE);
    Image image2 = GenImageColor(64, 64, Color(0, 0, 0, 0));
    ImageDrawCircle(&image2, 32, 32, 32, WHITE);
    instance->textures.emplace_back(LoadTextureFromImage(image1));
    instance->textures.emplace_back(LoadTextureFromImage(image2));
    RegisterSprite(0);
    RegisterSprite(1);

    instance->shaders.push_back(LoadShader("textures/shaders/default.vs", "textures/shaders/default.fs"));
}

void SpriteManager::LoadTexturesFromText(const char* path) {
    std::ifstream file(path);
    std::string line;
    
    std::string name;
    std::string data;

    float rect[4];
    std::uint8_t rectStage = 0;

    bool textureRegistering = true;
    bool readingName = false;
    bool nameRead = false;
    bool quotesOpened = false;

    while (std::getline(file, line)) {
        name.clear();
        data.clear();
        nameRead = false;
        readingName = false;
        rectStage = 0;

        for(auto c : line) {
            if(c == '\"') {
                quotesOpened = !quotesOpened;
                continue;
            }

            if(c == ' ' && quotesOpened == false) {
                if(nameRead == false) {
                    nameRead = true;
                } else if(textureRegistering == false && rectStage < 4) {
                    std::cout << line << std::endl;
                    rect[rectStage] = std::stof(data);
                    data.clear();
                    rectStage++;
                }

                continue;
            }

            if(c == '#' && quotesOpened == false) {
                readingName = true;
                continue;
            }

            if(c == ':' && quotesOpened == false) {
                textureRegistering = false;
                break;
            }

            if(c == '/' && quotesOpened == false) {
                break;
            }

            if(nameRead) {
                data.push_back(c);
            } else {
                name.push_back(c);
            }
        }

        if(nameRead == false) {
            continue;
        }

        if(textureRegistering) {
            TextureID id = SpriteManager::RegisterTexture(data.c_str()); 
            instance->namesMap[name] = id;
        } else {
            TextureID textureId = readingName ? instance->namesMap[name] : std::stoi(name);
            
            if(rectStage == 2) {
                rect[2] = std::stof(data);
            }
            
            rect[3] = std::stof(data);
            if(rectStage >= 2) {
                SpriteManager::RegisterSprite(textureId, { rect[0], rect[1], rect[2], rect[3] });
            } else {
                SpriteManager::RegisterSprite(textureId);
            }
        }
    }

    file.close();
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

Texture2D& SpriteManager::GetTexture(const char* name) {
    return GetTexture(instance->namesMap[name]);
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