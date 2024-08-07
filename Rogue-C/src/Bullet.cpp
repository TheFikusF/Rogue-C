#include "Bullet.h"
#include "Enemy.h"
#include "ParticleSystem.h"
#include "SpinningSphere.h"

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
        collider.velocity = bullet.direction * bullet.speed;
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
        ParticleSystemSystem::Spawn(collision.pointA, false);
    }
}

Entity SpawnBullet(Vec2 position, Vec2 direction) { 
    Entity entity = ECS::CreateEntity();
    ECS::AddComponent<MTransform>(entity, MTransform( position, Vec2(5,5)));
    ECS::AddComponent<Bullet>(entity, Bullet{ .direction = direction, .speed = 300, .timer = 0 });
    ECS::AddComponent<Drawer>(entity, Drawer(1, YELLOW) );
    ECS::AddComponent<Collider2D>(entity, Collider2D(true, false, 0));

    return entity; 
}

Entity SpawnBulletWithOrbit(Vec2 position, Vec2 direction) { 
    Entity bullet = SpawnBullet(position, direction);
    SpawnSphere(bullet, 2.0f, 30.0f, 8.0f);
    return bullet;
}
