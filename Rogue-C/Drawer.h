#pragma once
#include "System.h"
#include "SpriteManager.h"
#include "./include/raylib/raylib.h"

struct Drawer {
    Color color;
    Sprite sprite;
    Rectangle sourceRect;

    Drawer() : sprite(0), color(WHITE) {}
    Drawer(const Color& color) : sprite(0), color(color) {}
    Drawer(const Sprite& sprite) 
        : sprite(sprite), 
        color(WHITE), 
        sourceRect(Rectangle(0, 0, SpriteManager::GetTexture(sprite).width, SpriteManager::GetTexture(sprite).height)) {}
};

class DrawerSystem : public System {
public:
	uint32_t drawTime;

public:
    DrawerSystem();
    
    void Update();
};