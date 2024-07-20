#pragma once
#include "System.h"
#include "SpriteManager.h"
#include "./include/raylib/raylib.h"

struct Drawer {
    bool isColor;
    Color color;
    Sprite sprite;
    Rectangle sourceRect;

    Drawer() : isColor(true), color(WHITE) {}
    Drawer(const Color& color) : isColor(true), color(color) {}
    Drawer(const Sprite& sprite) 
        : isColor(false), 
        sprite(sprite), 
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