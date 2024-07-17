#include "Enemy.h"
#include "ECS.h"
#include "Drawer.h"
#include "Physics.h"

EnemySystem::EnemySystem() {
    signature.set(ECS::GetComponentType<MTransform>());
    signature.set(ECS::GetComponentType<Enemy>());
    signature.set(ECS::GetComponentType<Drawer>());
    signature.set(ECS::GetComponentType<Collider2D>());

    _spawnTime = 0;
}

void EnemySystem::Spawn(Vec2 position) {
    Entity entity = ECS::CreateEntity();
    ECS::AddComponent<MTransform>(entity, MTransform{ position, Vec2(20, 20) });
    ECS::AddComponent<Enemy>(entity, Enemy{ .speed = 40, .health = Health{ .current = 5, .max = 5, .onDeath = [entity]() -> void { ECS::DestroyEntity(entity); }} });
    ECS::AddComponent<Drawer>(entity, Drawer(_defaultEnemySprite));
    ECS::AddComponent<Collider2D>(entity, Collider2D{ .isTrigger = false, .useGravity = false, .kinematic = true,  .mass = 5, .force = Vec2(), .velocity = Vec2() });
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
        const Enemy& enemy = ECS::GetComponent<Enemy>(entity);
        Collider2D& collider = ECS::GetComponent<Collider2D>(entity);

        collider.velocity = (playerPosition - tr.position).GetNormalized() * enemy.speed;
        
        //tr.position += (playerPosition - tr.position).GetNormalized() * enemy.speed * dt;
    }
}

void EnemySystem::SetUp(Entity player, Sprite defaultSprite) {
    _player = player;
    _defaultEnemySprite = defaultSprite;
}
