#include "UIDrawer.h"
#include "Collisions.h"
#include "Transform.h"

UIDrawerSystem::UIDrawerSystem() {
    signature.set(ECS::GetComponentType<UIDrawer>());
    signature.set(ECS::GetComponentType<MTransform>());
}

void UIDrawerSystem::Update(float dt) {
    Vec2 mousePos(GetMouseX(), GetMouseY());
    for (auto const& entity : Entities) {
        const MTransform& tr = ECS::GetComponent<MTransform>(entity);
        const UIDrawer& dr = ECS::GetComponent<UIDrawer>(entity);
        
        if (PointInRectangle(mousePos, tr.position, tr.scale) == false) {
            continue;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            for(auto pointerDown : dr.pointerDowns) {
                pointerDown->PointerDown(entity);
            }
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            for(auto pointerDown : dr.pointerUps) {
                pointerDown->PointerUp(entity);
            }
        }

        for(auto pointerDown : dr.pointerHovers) {
            pointerDown->PointerHover(entity);
        }
    }
}

void UIDrawerSystem::Draw() {
    for(auto const& entity : Entities) {
        MTransform& tr = ECS::GetComponent<MTransform>(entity);
        const UIDrawer& drawer = ECS::GetComponent<UIDrawer>(entity);
        Vec2 realpos = MTransformSystem::GetRealPosition(entity);

        Texture2D& tex = SpriteManager::GetTexture(drawer.sprite);
        Vector2 scale(tr.scale.x / tex.width, tr.scale.y / tex.height);
        
        DrawTexturePro(tex, drawer.sourceRect, 
            { realpos.x, realpos.y, tr.scale.x * 2.0f, tr.scale.y * 2.0f},
            { tr.scale.x, tr.scale.y }, tr.rotation,  drawer.color);
    }
}
