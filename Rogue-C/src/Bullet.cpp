#include "Bullet.h"
#include <Enemy.h>

//std::vector<Bullet> Bullet::bullets;
//
//Bullet::Bullet(Vec2 position, Vec2 direction, float speed) 
//    : position(position), direction(direction.GetNormalized()), speed(speed), timer(0), id(bullets.size()) {
//    bullets.push_back(*this);
//}
//
//void Bullet::Process(float ds) {
//    position += direction * speed * ds;
//    timer += ds;
//
//    for(Enemy &enemy : Enemy::enemies) {
//        if(Vec2::Distance(position, enemy.position) < enemy.size + 5) {
//            enemy.health.TakeDamage(1);
//            Destroy();
//            return;
//        }
//    }
//}
//
//void Bullet::Destroy() {
//    Bullet::bullets.erase(Bullet::bullets.begin() + id);
//}

BulletSystem::BulletSystem() {
    signature.set(ECS::GetComponentType<MTransform>());
    signature.set(ECS::GetComponentType<Bullet>());
    signature.set(ECS::GetComponentType<Drawer>());
}

void BulletSystem::Update(float dt) {
    std::vector<Entity> requireDeletion;
    for(auto const& entity : Entities) {
        MTransform& tr = ECS::GetComponent<MTransform>(entity);
        Bullet& bullet = ECS::GetComponent<Bullet>(entity);
        tr.position += bullet.direction * bullet.speed * dt;
        bullet.timer += dt;

        if(bullet.timer >= 3) {
            requireDeletion.push_back(entity);
        }

        // for(Enemy &enemy : Enemy::enemies) {
        //     if(Vec2::Distance(position, enemy.position) < enemy.size + 5) {
        //         enemy.health.TakeDamage(1);
        //         ECS::DestoryEntity(entity);
        //         return;
        //     }
        // }
    }

    for(auto const& entity : requireDeletion) {
        ECS::DestoryEntity(entity);
    }
}

Entity SpawnBullet(Vec2 position, Vec2 direction) { 
    Entity entity = ECS::CreateEntity();
    ECS::AddComponent<MTransform>(entity, MTransform{ .position = position, .scale = Vec2(5,5) });
    ECS::AddComponent<Bullet>(entity, Bullet{ .direction = direction, .speed = 100, .timer = 0 });
    ECS::AddComponent<Drawer>(entity, Drawer{ .color = YELLOW });

    return entity; 
}
