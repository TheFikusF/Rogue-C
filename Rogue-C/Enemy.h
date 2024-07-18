#pragma once
#include "LOG.h"
#include "Vec2.h"
#include "Health.h"
#include "System.h"
#include "Entity.h"
#include "Transform.h"
#include "SpriteManager.h"
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

    void SetUp(Entity player, Sprite defaultSprite);

    void OnCollision(const Collision2D& collision) override;
    
private:
    Sprite _defaultEnemySprite;
    Entity _player;
    float _spawnTime;
};