#pragma once
#include "Vec2.h"

struct Player {
    Vec2 position;
    float speed;
    bool canShoot;
    float shotCooldown;
};