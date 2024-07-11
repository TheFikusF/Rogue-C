#include "Drawer.h"
#include <ECS.h>
#include <Transform.h>

DrawerSystem::DrawerSystem() {
    signature.set(ECS::GetComponentType<MTransform>());
    signature.set(ECS::GetComponentType<Drawer>());
}

void DrawerSystem::Update(float dt) {
    for(auto const& entity : Entities) {
        const MTransform& tr = ECS::GetComponent<MTransform>(entity);
        const Drawer& drawer = ECS::GetComponent<Drawer>(entity);
        DrawCircle(tr.position.x, tr.position.y, tr.scale.x, drawer.color);
    }
}