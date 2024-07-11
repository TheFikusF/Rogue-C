#pragma once
#include "Vec2.h"
#include "System.h"
#include "Timer.h"

struct Player {
    float speed;
    bool canShoot;
    Timer shootCooldown;
};

class PlayerSystem : public System {
public:
    PlayerSystem();

    void Update(float dt);
};