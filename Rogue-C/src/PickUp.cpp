#include "PickUp.h"
#include "Drawer.h"
#include "Transform.h"

PickUpSystem::PickUpSystem() {
    signature.set(ECS::GetComponentType<MTransform>());
    signature.set(ECS::GetComponentType<Collider2D>());
    signature.set(ECS::GetComponentType<Drawer>());
    signature.set(ECS::GetComponentType<PickUp>());
}

void PickUpSystem::Spawn(Vec2 position) {
    Entity pickup = ECS::CreateEntity();
    ECS::AddComponent<MTransform>(pickup, MTransform{ .position = position, .scale = Vec2(7.5f, 7.5f) });
    ECS::AddComponent<Collider2D>(pickup, Collider2D(true, false, 0));
    ECS::AddComponent<Drawer>(pickup, Drawer(GREEN));
    ECS::AddComponent<PickUp>(pickup, PickUp{ Heal });
}

void PickUpSystem::OnTrigger(const Collision2D& collision) {
    const PickUp& pickup = ECS::GetComponent<PickUp>(collision.a);
    if(ECS::HasComponent<Player>(collision.b)) {
        Player& player = ECS::GetComponent<Player>(collision.b);
        switch (pickup.type)
        {
        case Heal: player.health.Heal(1); break;
        case DamageBust: player.health.Heal(1); break;
        }
        ECS::DestroyEntity(collision.a);
    }
}