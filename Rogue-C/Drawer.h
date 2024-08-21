#pragma once
#include "System.h"
#include "SpriteManager.h"
#include "./include/raylib/raylib.h"

struct Drawer {
    Color color;
    SpriteID sprite;
    ShaderID shader;

    Drawer() : sprite(0), color(WHITE), shader(0) {}
    Drawer(const Color& color) : sprite(0), color(color), shader(0) {}
    Drawer(const SpriteID& sprite) : sprite(sprite), color(WHITE), shader(0) {}
    Drawer(const SpriteID& sprite, const Color& color) : sprite(sprite), color(color), shader(0) {}
};

class DrawerSystem : public Core::System {
public:
	std::uint32_t drawTime;

public:
    DrawerSystem();
    
    void Draw() override;
};