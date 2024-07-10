#pragma once
#include "Vec2.h"
#include "Health.h"
#include <vector>

struct Enemy {
    float speed;
    float size;
    Health health;
};

class EnemySystem {
    void Update(float dt) {
        
    }
};