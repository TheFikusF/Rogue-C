#include "PickUp.h"
#include "Drawer.h"
#include "Transform.h"
#include "SpinningSphere.h"

PickUpSystem::PickUpSystem() {
    signature.set(ECS::GetComponentType<MTransform>());
    signature.set(ECS::GetComponentType<Collider2D>());
    signature.set(ECS::GetComponentType<Drawer>());
    signature.set(ECS::GetComponentType<PickUp>());
}

void PickUpSystem::Spawn(Vec2 position) {
    int type = GetRandomValue(0, 1);
    Entity pickup = ECS::CreateEntity();
    ECS::AddComponent<MTransform>(pickup, MTransform{ .position = position, .scale = Vec2(7.5f, 7.5f) });
    ECS::AddComponent<Collider2D>(pickup, Collider2D(true, false, 0));
    switch (type)
    {
    case 0:
        ECS::AddComponent<Drawer>(pickup, Drawer(GREEN));
        ECS::AddComponent<PickUp>(pickup, PickUp{ PickUpType::Heal });
        break;
    case 1:
        ECS::AddComponent<Drawer>(pickup, Drawer(ORANGE));
        ECS::AddComponent<PickUp>(pickup, PickUp{ PickUpType::SpinningSphere });
        break;    
    default:
        break;
    }
}

void PickUpSystem::OnTrigger(const Collision2D& collision) {
    const PickUp& pickup = ECS::GetComponent<PickUp>(collision.a);
    if(ECS::HasComponent<Player>(collision.b)) {
        Player& player = ECS::GetComponent<Player>(collision.b);
        switch (pickup.type)
        {
        case PickUpType::Heal: player.health.Heal(1); break;
        case PickUpType::DamageBust: player.health.Heal(1); break;
        case PickUpType::SpinningSphere: SpawnSphere(collision.b); break;
        }
        ECS::DestroyEntity(collision.a);
    }
}