#include "Drawer.h"
#include <ECS.h>
#include <Transform.h>

DrawerSystem::DrawerSystem() : drawTime(0) {
    signature.set(ECS::GetComponentType<MTransform>());
    signature.set(ECS::GetComponentType<Drawer>());
}

void DrawerSystem::Update() {
    auto start = std::chrono::high_resolution_clock::now();
    for(auto const& entity : Entities) {
        MTransform& tr = ECS::GetComponent<MTransform>(entity);
        const Drawer& drawer = ECS::GetComponent<Drawer>(entity);
        Vec2 realpos = MTransformSystem::GetRealPosition(entity);

        Texture2D& tex = SpriteManager::GetTexture(drawer.sprite);
        Vector2 scale(tr.scale.x / tex.width, tr.scale.y / tex.height);
        
        DrawTexturePro(tex, drawer.sourceRect, 
            { realpos.x, realpos.y, tr.scale.x * 2.0f, tr.scale.y * 2.0f},
            { tr.scale.x, tr.scale.y }, tr.rotation,  drawer.color);
    }
    auto end = std::chrono::high_resolution_clock::now();
    drawTime = (end - start).count();
}