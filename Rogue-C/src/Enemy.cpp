#include "Enemy.h"
#include "ECS.h"
#include "Drawer.h"
#include "Physics.h"
#include "Player.h"
#include "PickUp.h"

EnemySystem::EnemySystem() {
    signature.set(ECS::GetComponentType<MTransform>());
    signature.set(ECS::GetComponentType<Enemy>());
    signature.set(ECS::GetComponentType<Drawer>());
    signature.set(ECS::GetComponentType<Collider2D>());

    _spawnTime = 0;
}

void EnemySystem::Spawn(Vec2 position) {
    int enemy = GetRandomValue(0, 2);
    switch (enemy)
    {
    case 1: SpawnType(position, 10, 30, 30, _bigEnemySprite); break;
    case 2: SpawnType(position, 3, 60, 12, _smallEnemySprite); break;
    default: SpawnType(position, 5, 40, 20, _defaultEnemySprite); break;
    }
}


void EnemySystem::SpawnType(Vec2 position, int health, float speed, float size, Sprite sprite) {
    Entity entity = ECS::CreateEntity();
    ECS::AddComponent<MTransform>(entity, MTransform(position, Vec2(size, size)));
    ECS::AddComponent<Enemy>(entity, Enemy{ .speed = speed, .health = Health(health, 0.05f, [entity]() -> void { 
        const MTransform& tr = ECS::GetComponent<MTransform>(entity);
        ECS::DestroyEntity(entity); 
        PickUpSystem::Spawn(tr.position);
    }) } );
    ECS::AddComponent<Drawer>(entity, Drawer(sprite));
    ECS::AddComponent<Collider2D>(entity, Collider2D(false, false, 5));
}

void EnemySystem::Update(float dt) {
    _spawnTime += dt;

    if (_spawnTime >= 3) {
        _spawnTime = 0;
        long r = GetRandomValue(0, 360);
        int width = GetRenderWidth() / 2;
        int height = GetRenderHeight() / 2;
        Spawn(Vec2(width + sin(r) * width, height + cos(r) * height));
        LOG_WARNING("SPAWNED enemy OMG");
    }

    Vec2 playerPosition = ECS::GetComponent<MTransform>(_player).position;
    for (auto const& entity : Entities) {
        MTransform& tr = ECS::GetComponent<MTransform>(entity);
        Enemy& enemy = ECS::GetComponent<Enemy>(entity);
        Collider2D& collider = ECS::GetComponent<Collider2D>(entity);

        collider.velocity = (playerPosition - tr.position).GetNormalized() * enemy.speed;
        
        enemy.health.Process(dt);
        //tr.position += (playerPosition - tr.position).GetNormalized() * enemy.speed * dt;
    }
}

void EnemySystem::SetUp(Entity player, Sprite defaultSprite, Sprite bigSprite, Sprite smallSprite) {
    _player = player;
    _defaultEnemySprite = defaultSprite;
    _bigEnemySprite = bigSprite;
    _smallEnemySprite = smallSprite;
}

void EnemySystem::OnCollision(const Collision2D& collision) {
    if(ECS::HasComponent<Player>(collision.b)) {
        Player& player = ECS::GetComponent<Player>(collision.b);
        player.health.TakeDamage(1);
    }
}
