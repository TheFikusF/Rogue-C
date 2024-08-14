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

class EnemySystem : public System {
public:
    EnemySystem();

    void Spawn(Vec2 position);
    
    void Update(float dt) override;

    void SetUp(Entity player, Sprite defaultSprite, Sprite bigSprite, Sprite smallSprite);

    void OnCollision(const Collision2D& collision) override;
    
private:
    Sprite _defaultEnemySprite;
    Sprite _bigEnemySprite;
    Sprite _smallEnemySprite;
    Entity _player;
    float _spawnTime;

    void SpawnType(Vec2 position, int health, float speed, float size, Sprite sprite);
};