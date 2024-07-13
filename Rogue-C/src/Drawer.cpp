#include "Drawer.h"
#include <ECS.h>
#include <Transform.h>

DrawerSystem::DrawerSystem() {
    signature.set(ECS::GetComponentType<MTransform>());
    signature.set(ECS::GetComponentType<Drawer>());
}

void DrawerSystem::Update(float dt) {
    for(auto const& entity : Entities) {
        MTransform& tr = ECS::GetComponent<MTransform>(entity);
        const Drawer& drawer = ECS::GetComponent<Drawer>(entity);
        Vec2 realpos = MTransformSystem::GetRealPosition(entity);
        DrawCircle(realpos.x, realpos.y, tr.scale.x, drawer.color);
//        DrawCircle(tr.position.x, tr.position.y, tr.scale.x, drawer.color);
    }
}