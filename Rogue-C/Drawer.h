#pragma once
#include "System.h"
#include "SpriteManager.h"
#include "./include/raylib/raylib.h"

struct Drawer {
    Color color;
    SpriteID sprite;

    Drawer() : sprite(0), color(WHITE) {}
    Drawer(const Color& color) : sprite(0), color(color) {}
    Drawer(const SpriteID& sprite) 
        : sprite(sprite), 
        color(WHITE) {}

    Drawer(const SpriteID& sprite, const Color& color)
        : sprite(sprite),
        color(color) {}
};

class DrawerSystem : public Core::System {
public:
	std::uint32_t drawTime;

public:
    DrawerSystem();
    
    void Draw() override;
};