#include "Enemy.h"
#include "ECS.h"
#include "Drawer.h"

// void Enemy::Die() {
//     //enemies.erase(enemies.begin() + id);
// }

// void Enemy::DrawAll() {
//     for(const Enemy &enemy : enemies) {
//         DrawCircle(enemy.position.x, enemy.position.y, enemy.size, RED);
//     }
// }

EnemySystem::EnemySystem() {
    signature.set(ECS::GetComponentType<MTransform>());
    signature.set(ECS::GetComponentType<Enemy>());
    signature.set(ECS::GetComponentType<Drawer>());
}

void EnemySystem::Spawn(Vec2 position) {
    Entity entity = ECS::CreateEntity();
    ECS::AddComponent<MTransform>(entity, MTransform{ position, Vec2(20, 20) });
    ECS::AddComponent<Enemy>(entity, Enemy{ .speed = 40, .health = Health{ .current = 5, .max = 5, .onDeath = [entity]() -> void { ECS::DestoryEntity(entity); }} });
    ECS::AddComponent<Drawer>(entity, Drawer{ RED });
}

void EnemySystem::Update(float dt) {
    Vec2 playerPosition = ECS::GetComponent<MTransform>(_player).position;
    for (auto const& entity : Entities) {
        MTransform& tr = ECS::GetComponent<MTransform>(entity);
        const Enemy& enemy = ECS::GetComponent<Enemy>(entity);
        tr.position += (playerPosition - tr.position).GetNormalized() * enemy.speed * dt;
    }
}

void EnemySystem::Draw() {
    for (auto const& entity : Entities) {
        const MTransform& tr = ECS::GetComponent<MTransform>(entity);
        const Enemy& enemy = ECS::GetComponent<Enemy>(entity);
        DrawCircle(tr.position.x, tr.position.y, tr.scale.x, RED);
    }
}

void EnemySystem::SetPlayer(Entity player) {
    _player = player;
}
