#include "PickUp.h"

void PickUpSystem::OnTrigger(const Collision2D& collision) {
    if(ECS::HasComponent<Player>(collision.b)) {
        Player& player = ECS::GetComponent<Player>(collision.b);
        ECS::DestroyEntity(collision.a);
    }
}