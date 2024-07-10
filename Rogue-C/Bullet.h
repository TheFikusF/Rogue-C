#pragma once
#include "Vec2.h"
#include <vector>

class Bullet {
public:
    int id;
    Vec2 position;
    Vec2 direction;
    float speed;
    float timer;

    static std::vector<Bullet> bullets;

public:
    Bullet(Vec2 position, Vec2 direction, float speed);

    //~Bullet() { std::cout << "bullet gone" << std::endl; }

    void Process(float ds);
    void Destroy();
};
