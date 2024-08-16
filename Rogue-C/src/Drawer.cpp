#include "Drawer.h"
#include <ECS.h>
#include <Transform.h>

using namespace Core;

DrawerSystem::DrawerSystem() : drawTime(0) {
    signature.set(ECS::GetComponentType<MTransform>());
    signature.set(ECS::GetComponentType<Drawer>());
}

void DrawerSystem::Draw() {
    auto start = std::chrono::high_resolution_clock::now();
    for(auto const& entity : Entities) {
        MTransform& tr = ECS::GetComponent<MTransform>(entity);
        const Drawer& drawer = ECS::GetComponent<Drawer>(entity);
        Vec2 realpos = MTransformSystem::GetRealPosition(entity);

        const Sprite& sprite = SpriteManager::GetSprite(drawer.sprite);
        const Texture2D& tex = SpriteManager::GetTexture(sprite.texture);

        Vector2 scale(tr.scale.x / tex.width, tr.scale.y / tex.height);
        
        DrawTexturePro(tex, sprite.rect, 
            { realpos.x, realpos.y, tr.scale.x * 2.0f, tr.scale.y * 2.0f},
            { tr.scale.x, tr.scale.y }, tr.rotation,  drawer.color);
    }
    auto end = std::chrono::high_resolution_clock::now();
    drawTime = (end - start).count();
}