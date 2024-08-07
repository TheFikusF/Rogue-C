#include "Player.h"
#include "InputSystem.h"
#include "Bullet.h"
#include "SpinningSphere.h"
#include <iostream>
#include "Transform.h"
#include "ECS.h"
#include "Drawer.h"
#include "Physics.h"
#include "AudioManager.h"

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
        
        col.velocity = InputSystem::GetMovementAxis() * pl.speed;

        pl.health.Process(dt);
        if(IsKeyPressed(KEY_Q) && pl.shootCooldown.IsStarted() == false) {
            pl.abilityAmplitude.Stop();

            pl.shootCooldown.Start();
            pl.abilityAmplitude.Start();
            pl.abilityDuration.Start();
        }

        if(pl.abilityDuration.InvCheck(dt) && pl.abilityAmplitude.Check(dt)) {
            pl.abilityAmplitude.Start();
            SpawnBulletWithOrbit(tr.position, InputSystem::GetShootingAxis());
            AudioManager::Play(0);
            LOG("spawned orbiting bullet");
        }

        if(pl.shootCooldown.InvCheck(dt) == false && InputSystem::IsShooting()) {
            SpawnBullet(tr.position, InputSystem::GetShootingAxis());
            //SpawnSphere(player);
            pl.shootCooldown.Start();
        }
    }
}
