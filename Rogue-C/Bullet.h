#pragma once
#include "core.h"

struct Bullet {
    Vec2 direction;
    float speed;
    float timer;
};

class BulletSystem : public Core::System {
public:
    BulletSystem();

    void Update(float dt) override;
    void OnTrigger(const Collision2D& collision) override;
};

Entity SpawnBullet(Vec2 position, Vec2 direction);
Entity SpawnBulletWithOrbit(Vec2 position, Vec2 direction);
