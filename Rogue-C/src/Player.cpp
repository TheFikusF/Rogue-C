#include "Player.h"
#include "Input.h"
#include "Bullet.h"
#include <iostream>
#include "Transform.h"
#include "ECS.h"
#include "Drawer.h"

//void Player::Process(float ds) {
//    position += Input::GetMovementAxis() * ds * speed;
//
//    if(Input::IsShooting() && canShoot) {
//        Bullet bullet(position, Input::GetShootingAxis(), 100);
//        canShoot = false;
//        Timer timer(shotCooldown, [this]() -> void { this->canShoot = true; });
//    }
//}

PlayerSystem::PlayerSystem() {
    signature.set(ECS::GetComponentType<MTransform>());
    signature.set(ECS::GetComponentType<Player>());
    signature.set(ECS::GetComponentType<Drawer>());
}

void PlayerSystem::Update(float dt) {
    for (Entity const& player : Entities) {
        MTransform& tr = ECS::GetComponent<MTransform>(player);
        Player& pl = ECS::GetComponent<Player>(player);
        tr.position += Input::GetMovementAxis() * dt * pl.speed;

        pl.shootCooldown.Process(dt);
        if(pl.shootCooldown.GetFinished() == true) {
            pl.canShoot = true;
        }

        if(Input::IsShooting() && pl.canShoot) {
            SpawnBullet(tr.position, Input::GetShootingAxis());
            pl.canShoot = false;
            pl.shootCooldown.Start();
        }
    }
}
