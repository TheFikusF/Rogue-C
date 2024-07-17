#pragma once
#include "System.h"
#include "SpriteManager.h"
#include "./include/raylib/raylib.h"

struct Drawer {
    bool isColor;
    Color color;
    Sprite sprite;

    Drawer() : isColor(true), color(WHITE) {}
    Drawer(const Color& color) : isColor(true), color(color) {}
    Drawer(const Sprite& sprite) : isColor(false), sprite(sprite), color(WHITE) {}
};

class DrawerSystem : public System {
public:
	uint32_t drawTime;

public:
    DrawerSystem();
    
    void Update();
};