#pragma once
#include "Vec2.h"
#include <vector>

class Bullet {
public:
    Vec2 position;
    Vec2 direction;
    float speed;
    float timer;

    static std::vector<Bullet> bullets;

public:
    Bullet(Vec2 position, Vec2 direction, float speed)
        : position(position), direction(direction.GetNormalized()), speed(speed), timer(0) { }

    //~Bullet() { std::cout << "bullet gone" << std::endl; }

    void Process(float ds);
};
