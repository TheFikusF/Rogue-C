#pragma once
#include "Vec2.h"
#include "System.h"
#include "ECS.h"
#include "Drawer.h"

struct Bullet {
    Vec2 direction;
    float speed;
    float timer;
};

class BulletSystem : public System {
public:
    BulletSystem();

    void Update(float dt);
};

Entity SpawnBullet(Vec2 position, Vec2 direction);
