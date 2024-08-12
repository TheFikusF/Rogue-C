#pragma once
#include "UIDrawer.h"

struct Button : public IPointerDown, public IPointerUp {
    void PointerDown(Entity entity) override;
    void PointerUp(Entity entity) override;
};

class ButtonSystem : public PointerCallbackSystem<Button> { 
    //ButtonSystem() { }
};