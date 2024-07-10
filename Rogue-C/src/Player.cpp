#include "Player.h"
#include "Input.h"
#include "Bullet.h"
#include "Timer.h"
#include <iostream>

void Player::Process(float ds) {
    position += Input::GetMovementAxis() * ds * speed;

    if(Input::IsShooting() && canShoot) {
        Bullet bullet(position, Input::GetShootingAxis(), 100);
        canShoot = false;
        Timer timer(shotCooldown, [this]() -> void { this->canShoot = true; });
    }
}
