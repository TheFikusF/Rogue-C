#pragma once
#include "Vec2.h"

class Player {
public:
    Vec2 position;
    float speed;
    bool canShoot;
    float shotCooldown;

public:
    Player(Vec2 position, float speed) 
        : position(position), speed(speed), shotCooldown(0.2f), canShoot(true) { }

    void Process(float ds);
};