#pragma once
#include "System.h"
#include "./include/raylib/raylib.h"

struct Drawer
{
    Color color;
};

class DrawerSystem : public System {
public:
    DrawerSystem();
    
    void Update(float dt);
};