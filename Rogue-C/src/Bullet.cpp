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
