#include "Bullet.h"

std::vector<Bullet> Bullet::bullets;

void Bullet::Process(float ds){
    position += direction * speed * ds;
    timer += ds;
}