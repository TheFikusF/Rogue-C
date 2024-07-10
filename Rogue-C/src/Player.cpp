#include "Player.h"
#include "Input.h"
#include "Bullet.h"
#include "Timer.h"
#include <iostream>

void Player::Process(float ds) {
    position += Input::GetMovementAxis() * ds * speed;

    if(Input::IsShooting() && canShoot) {
        Bullet::bullets.push_back(Bullet(position, Input::GetShootingAxis(), 100));
        canShoot = false;
        Timer timer(shotCooldown, [this]() -> void { 
            this->canShoot = true; 
            std::cout << "ready" << std::endl;
        });
    }
}
