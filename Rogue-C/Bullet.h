#pragma once
#include "Vec2.h"

struct Bullet {
    Vec2 position;
    Vec2 direction;
    float speed;
    float timer;
};
