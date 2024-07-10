#include "Player.h"
#include "Input.h"
#include "Bullet.h"
#include "Timer.h"
#include <iostream>
#include "Transform.h"
#include <ECS.h>

//void Player::Process(float ds) {
//    position += Input::GetMovementAxis() * ds * speed;
//
//    if(Input::IsShooting() && canShoot) {
//        Bullet bullet(position, Input::GetShootingAxis(), 100);
//        canShoot = false;
//        Timer timer(shotCooldown, [this]() -> void { this->canShoot = true; });
//    }
//}

void PlayerSystem::Update(float dt) {
	for (Entity const& player : Entities) {
		MTransform& tr = ECS::Instance.GetComponent<MTransform>(player);
		Player& pl = ECS::Instance.GetComponent<Player>(player);
		tr.position += Input::GetMovementAxis() * dt * pl.speed;
	}
}
