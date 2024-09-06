#pragma once
#include "./include/core/rendering/UIDrawer.h"
#include "./include/core/systems/Tween.h"
#include <functional>

struct Button : public IPointerDown, public IPointerUp {
    Vec2 scale;
    TweenID _tween;
    std::function<void (void)> onClick;
    std::function<void (void)> onUp;

    Button();
    Button(Vec2 scale, void (*onClick) (void));

    void PointerDown(Entity entity) override;
    void PointerUp(Entity entity) override;
};

class ButtonSystem : public PointerCallbackSystem<Button> { };