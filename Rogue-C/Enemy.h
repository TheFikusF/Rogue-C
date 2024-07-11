#pragma once
#include "Vec2.h"
#include "Health.h"
#include "System.h"
#include "Entity.h"
#include "Transform.h"
#include <vector>
#include <memory>

struct Enemy {
    float speed;
    Health health{};
};

class EnemySystem : public System {
public:
    EnemySystem();

    void Spawn(Vec2 position);
    
    void Update(float dt);

    void Draw();

    void SetPlayer(Entity player);
    
private:
    Entity _player;
};