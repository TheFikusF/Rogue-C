#include "Player.h"
#include "Input.h"
#include "Bullet.h"
#include "SpinningSphere.h"
#include <iostream>
#include "Transform.h"
#include "ECS.h"
#include "Drawer.h"
#include "Physics.h"

PlayerSystem::PlayerSystem() {
    signature.set(ECS::GetComponentType<MTransform>());
    signature.set(ECS::GetComponentType<Player>());
    signature.set(ECS::GetComponentType<Drawer>());
    signature.set(ECS::GetComponentType<Collider2D>());
}

void PlayerSystem::Update(float dt) {
    for (Entity const& player : Entities) {
        MTransform& tr = ECS::GetComponent<MTransform>(player);
        Player& pl = ECS::GetComponent<Player>(player);
        Collider2D& col = ECS::GetComponent<Collider2D>(player);
        
        col.velocity = Input::GetMovementAxis() * dt * pl.speed;

        if(pl.shootCooldown.Check(dt) == true) {
            pl.canShoot = true;
        }

        if(Input::IsShooting() && pl.canShoot) {
            //SpawnBullet(tr.position, Input::GetShootingAxis());
            SpawnSphere(player);
            pl.canShoot = false;
            pl.shootCooldown.Start();
        }
    }
}
