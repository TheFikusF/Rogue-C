#include "Bullet.h"
#include <Enemy.h>
#include <iostream>

BulletSystem::BulletSystem() {
    signature.set(ECS::GetComponentType<MTransform>());
    signature.set(ECS::GetComponentType<Bullet>());
    signature.set(ECS::GetComponentType<Drawer>());
    signature.set(ECS::GetComponentType<Collider2D>());
}

void BulletSystem::Update(float dt) {
    std::vector<Entity> requireDeletion;
    for(auto const& entity : Entities) {
        MTransform& tr = ECS::GetComponent<MTransform>(entity);
        Bullet& bullet = ECS::GetComponent<Bullet>(entity);
        Collider2D& collider = ECS::GetComponent<Collider2D>(entity);
        collider.velocity = bullet.direction * bullet.speed * dt;
        bullet.timer += dt;

        if(bullet.timer >= 3) {
            requireDeletion.push_back(entity);
        }
    }

    for(auto const& entity : requireDeletion) {
        ECS::DestroyEntity(entity);
    }
}

void BulletSystem::OnTrigger(const Collision2D& collision) {
    if(ECS::HasComponent<Enemy>(collision.b)) {
        Enemy& enemy = ECS::GetComponent<Enemy>(collision.b);
        enemy.health.TakeDamage(1);
        ECS::DestroyEntity(collision.a);
    }
}

Entity SpawnBullet(Vec2 position, Vec2 direction) { 
    Entity entity = ECS::CreateEntity();
    ECS::AddComponent<MTransform>(entity, MTransform{ .position = position, .scale = Vec2(5,5) });
    ECS::AddComponent<Bullet>(entity, Bullet{ .direction = direction, .speed = 100, .timer = 0 });
    ECS::AddComponent<Drawer>(entity, Drawer{ .color = YELLOW });
    ECS::AddComponent<Collider2D>(entity, Collider2D{ .isStatic = false, .isTrigger = true, .useGravity = false, .kinematic = false, .mass = 5, .force = Vec2(), .velocity = Vec2() });

    return entity; 
}


