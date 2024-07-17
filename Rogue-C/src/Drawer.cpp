#include "Drawer.h"
#include <ECS.h>
#include <Transform.h>

DrawerSystem::DrawerSystem() {
    drawTime = 0;
    signature.set(ECS::GetComponentType<MTransform>());
    signature.set(ECS::GetComponentType<Drawer>());
}

void DrawerSystem::Update() {
    LOG("drawing");
    auto start = std::chrono::high_resolution_clock::now();
    for(auto const& entity : Entities) {
        MTransform& tr = ECS::GetComponent<MTransform>(entity);
        const Drawer& drawer = ECS::GetComponent<Drawer>(entity);
        LOG("getting real pos");
        Vec2 realpos = MTransformSystem::GetRealPosition(entity);
        DrawCircle(realpos.x, realpos.y, tr.scale.x, drawer.color);
//        DrawCircle(tr.position.x, tr.position.y, tr.scale.x, drawer.color);
    }
    auto end = std::chrono::high_resolution_clock::now();
    drawTime = (end - start).count();
    LOG("drawing end");
}