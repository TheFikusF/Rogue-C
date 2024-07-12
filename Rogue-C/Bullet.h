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
    void OnTrigger(const Collision2D& collision) override;
};

Entity SpawnBullet(Vec2 position, Vec2 direction);
