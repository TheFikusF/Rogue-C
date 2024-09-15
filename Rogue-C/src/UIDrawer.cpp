#include "./include/core/rendering/UIDrawer.h"
#include "./include/core/rendering/ESRendering.h"
#include "./include/core/physics/Collisions.h"
#include "./include/core/Transform.h"

using namespace Core;
using namespace Rendering;

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
            for (auto pointerDown : dr.pointerDowns) {
                pointerDown->PointerDown(entity);
            }
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            for (auto pointerDown : dr.pointerUps) {
                pointerDown->PointerUp(entity);
            }
        }

        for (auto pointerDown : dr.pointerHovers) {
            pointerDown->PointerHover(entity);
        }
    }
}

void UIDrawerSystem::Draw() {
    //_entitiesTemp.assign(Entities.begin(), Entities.end());
    _entitiesTemp.clear();
    std::copy(Entities.begin(), Entities.end(), std::back_inserter(_entitiesTemp));
    std::sort(_entitiesTemp.begin(), _entitiesTemp.end(), SortCustom);

    for (auto const& entity : _entitiesTemp) {
        MTransform& tr = ECS::GetComponent<MTransform>(entity);
        const UIDrawer& drawer = ECS::GetComponent<UIDrawer>(entity);
        Vec2 realpos = MTransformSystem::GetRealPosition(entity);

        ESRenderer::PushUI(drawer.order, drawer.sprite, MTransform(realpos, tr.scale, tr.rotation), drawer.color);
    }
}
