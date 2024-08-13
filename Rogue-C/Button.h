#pragma once
#include "UIDrawer.h"
#include <functional>
#include "Tween.h"

struct Button : public IPointerDown, public IPointerUp {
    Vec2 scale;
    Tween<Vec2>* _tween;
    std::function<void (void)> onClick;
    std::function<void (void)> onUp;

    Button();
    Button(Vec2 scale, void (*onClick) (void));

    void PointerDown(Entity entity) override;
    void PointerUp(Entity entity) override;
};

class ButtonSystem : public PointerCallbackSystem<Button> { 
    //ButtonSystem() { }
};