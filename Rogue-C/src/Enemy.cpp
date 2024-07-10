#include "Enemy.h"
#include "./include/raylib/raylib.h"
#include "ECS.h"

// void Enemy::Die() {
//     //enemies.erase(enemies.begin() + id);
// }

// void Enemy::DrawAll() {
//     for(const Enemy &enemy : enemies) {
//         DrawCircle(enemy.position.x, enemy.position.y, enemy.size, RED);
//     }
// }

void EnemySystem::Spawn(Vec2 position) {
    Entity entity = ECS::Instance.CreateEntity();
    ECS::Instance.AddComponent<MTransform>(entity, { position, Vec2(20, 20) });
    ECS::Instance.AddComponent<Enemy>(entity, { 40, Health(5, [entity]() -> void { ECS::Instance.DestoryEntity(entity); }) });
}

void EnemySystem::Update(float dt) {
    for (auto const& entity : Entities) {
        MTransform& tr = ECS::Instance.GetComponent<MTransform>(entity);
        const Enemy& enemy = ECS::Instance.GetComponent<Enemy>(entity);
        tr.position += (_player->position - tr.position).GetNormalized() * enemy.speed * dt;
    }
}

void EnemySystem::Draw() {
    for (auto const& entity : Entities) {
        const MTransform& tr = ECS::Instance.GetComponent<MTransform>(entity);
        const Enemy& enemy = ECS::Instance.GetComponent<Enemy>(entity);
        DrawCircle(tr.position.x, tr.position.y, tr.scale.x, RED);
    }
}

void EnemySystem::SetPlayer(MTransform* player) {
    _player = player;
}
