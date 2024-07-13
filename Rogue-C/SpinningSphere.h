#pragma once
#include "System.h"
#include "ECS.h"
#include "Transform.h"
#include "Collisions.h"
#include "Enemy.h"
#include "Drawer.h"

struct SpinningSphere {
    float radius;
    float speed;
    float timer;
};

class SpinningSphereSystem : public System {
public:
    SpinningSphereSystem();
    void Update(float dt);
    void OnTrigger(const Collision2D& collision) override;
};

void SpawnSphere(const Entity& parent);
