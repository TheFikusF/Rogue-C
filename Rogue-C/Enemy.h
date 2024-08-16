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
#include "Tween.h"

struct Enemy {
    TweenID sequence;
    float speed;
    Health health;

    Enemy();
    Enemy(Entity entity, float size, float speed, int health);
};

class EnemySystem : public Core::System {
public:
    EnemySystem();

    void Spawn(Vec2 position);
    
    void Update(float dt) override;

    void SetUp(Entity player, SpriteID defaultSprite, SpriteID bigSprite, SpriteID smallSprite);

    void OnCollision(const Collision2D& collision) override;
    
private:
    SpriteID _defaultEnemySprite;
    SpriteID _bigEnemySprite;
    SpriteID _smallEnemySprite;
    Entity _player;
    float _spawnTime;

    void SpawnType(Vec2 position, int health, float speed, float size, SpriteID sprite);
};