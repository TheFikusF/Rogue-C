#pragma once
#include "System.h"
#include "./include/raylib/raylib.h"

struct Drawer
{
    Color color;
};

class DrawerSystem : public System {
public:
	uint32_t drawTime;

public:
    DrawerSystem();
    
    void Update();
};