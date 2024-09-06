#include "./include/core/rendering/Drawer.h"
#include "./include/core/ecs/ECS.h"
#include "./include/core/Transform.h"
#include "./include/core/systems/CameraControl.h"

using namespace Core;
using namespace Rendering;

template<>
void Serialization::Read<Color>(const Serialization::Node* current, Color& target) {
    if(current->name == "r") target.r = std::stoi(current->value);
    if(current->name == "g") target.g = std::stoi(current->value);
    if(current->name == "b") target.b = std::stoi(current->value);
    if(current->name == "a") target.a = std::stoi(current->value);
}

template<>
void Serialization::Write<Color>(Serialization::Node* current, const Color& from) {
    current->AddChild("r", std::to_string(from.r));
    current->AddChild("g", std::to_string(from.g));
    current->AddChild("b", std::to_string(from.b));
    current->AddChild("a", std::to_string(from.a));
}

Drawer::Drawer() : sprite(0), color(WHITE), shader(0), order(0) {}
Drawer::Drawer(const Color& color) : sprite(0), color(color), shader(0), order(0) {}
Drawer::Drawer(const SpriteID& sprite) : sprite(sprite), color(WHITE), shader(0), order(0) {}
Drawer::Drawer(const SpriteID& sprite, const Color& color) : sprite(sprite), color(color), shader(0), order(0) {}

template<>
void Serialization::Read<Drawer>(const Serialization::Node* current, Drawer& target) {
    if(current->name.compare("color") == 0) target.color = current->Read<Color>();
    if(current->name.compare("sprite") == 0) target.sprite = std::stoul(current->value);
    if(current->name.compare("shader") == 0) target.shader = std::stoul(current->value);
    if(current->name.compare("order") == 0) target.order = std::stof(current->value);
}

template<>
void Serialization::Write<Drawer>(Serialization::Node* current, const Drawer& from) {
    Serialization::Write(current->AddChild("color"), from.color);
    current->AddChild("sprite", std::to_string(from.sprite));
    current->AddChild("shader", std::to_string(from.shader));
    current->AddChild("order", std::to_string(from.order));
}

DrawerSystem::DrawerSystem() : drawTime(0), drawOrder(DrawOrder::YAscending) {
    signature.set(ECS::GetComponentType<MTransform>());
    signature.set(ECS::GetComponentType<Drawer>());
}

void DrawerSystem::Draw() {
    auto start = std::chrono::high_resolution_clock::now();
    //TODO: adding check for bounds and sorting only entities in bounds
    _entitiesTemp.clear();
    for(auto const& entity : Entities) {
        _entitiesTemp.push_back(entity);
    }

    switch (drawOrder)
    {
    case DrawOrder::YAscending: std::sort(_entitiesTemp.begin(), _entitiesTemp.end(), SortYAsc); break;
    case DrawOrder::YDescending: std::sort(_entitiesTemp.begin(), _entitiesTemp.end(), SortYDesc); break;
    default: std::sort(_entitiesTemp.begin(), _entitiesTemp.end(), SortCustom); break;
    }

    BeginMode2D(CameraContorl::GetCurrent());
    for(auto const& entity : _entitiesTemp) {
        MTransform& tr = ECS::GetComponent<MTransform>(entity);
        const Drawer& drawer = ECS::GetComponent<Drawer>(entity);
        Vec2 realpos = MTransformSystem::GetRealPosition(entity);

        const Sprite& sprite = SpriteManager::GetSprite(drawer.sprite);
        const Texture2D& tex = SpriteManager::GetTexture(sprite.texture);

        Vector2 scale(tr.scale.x / tex.width, tr.scale.y / tex.height);
        
        BeginShaderMode(SpriteManager::GetShader(drawer.shader));
        DrawTexturePro(tex, sprite.rect, 
            { realpos.x, realpos.y, tr.scale.x * 2.0f, tr.scale.y * 2.0f },
            { tr.scale.x, tr.scale.y }, tr.rotation,  drawer.color);
        
        EndShaderMode();
    }
    auto end = std::chrono::high_resolution_clock::now();
    drawTime = (end - start).count();
    EndMode2D();
}

bool Rendering::SortYAsc(Entity a, Entity b) {
    const Vec2 posA = MTransformSystem::GetRealPosition(a);
    const Vec2 posB = MTransformSystem::GetRealPosition(b);
    return posA.y < posB.y;
}

bool Rendering::SortYDesc(Entity a, Entity b) { 
    const Vec2 posA = MTransformSystem::GetRealPosition(a);
    const Vec2 posB = MTransformSystem::GetRealPosition(b);
    return posA.y > posB.y;
}

bool Rendering::SortCustom(Entity a, Entity b) { 
    const float orderA = ECS::GetComponent<Drawer>(a).order;
    const float orderB = ECS::GetComponent<Drawer>(b).order;
    return orderA < orderB;
}