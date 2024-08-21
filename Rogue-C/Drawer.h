#pragma once
#include "System.h"
#include "SpriteManager.h"
#include "./include/raylib/raylib.h"

enum class DrawOrder {
    YAscending,
    YDescending,
    CustomOrder,
};

struct Drawer {
    Color color;
    SpriteID sprite;
    ShaderID shader;
    float order;

    Drawer() : sprite(0), color(WHITE), shader(0), order(0) {}
    Drawer(const Color& color) : sprite(0), color(color), shader(0), order(0) {}
    Drawer(const SpriteID& sprite) : sprite(sprite), color(WHITE), shader(0), order(0) {}
    Drawer(const SpriteID& sprite, const Color& color) : sprite(sprite), color(color), shader(0), order(0) {}
};

class DrawerSystem : public Core::System {
public:
    DrawOrder drawOrder;
	std::uint32_t drawTime;

public:
    DrawerSystem();
    
    void Draw() override;

private:
    std::vector<Entity> _entitiesTemp;

    static bool SortYAsc(Entity a, Entity b);
    static bool SortYDesc(Entity a, Entity b);
    static bool SortCustom(Entity a, Entity b);
};