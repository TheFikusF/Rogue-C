#pragma once
#include "core.h"
#include "Enemy.h"

struct SpinningSphere {
    float radius;
    float speed;
    float timer;
};

class SpinningSphereSystem : public Core::System {
public:
    SpinningSphereSystem();
    void Update(float dt) override;
    void OnTrigger(const Collision2D& collision) override;
};

void SpawnSphere(const Entity& parent, float size = 5, float speed = 20, float radius = 30);
