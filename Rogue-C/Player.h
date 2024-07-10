#pragma once
#include "Vec2.h"
#include "System.h"

struct Player {
    float speed;
    bool canShoot;
    float shotCooldown;
};

class PlayerSystem : public System {
public:
    void Update(float dt);
};