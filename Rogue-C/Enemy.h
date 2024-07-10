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
    Health health;

    Enemy() = default;
};

class EnemySystem : public System {
public:
    void Spawn(Vec2 position);
    
    void Update(float dt);

    void Draw();

    void SetPlayer(MTransform* player);
    
private:
    MTransform* _player;
};