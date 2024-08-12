#include "Button.h"
#include "LOG.h"

void Button::PointerDown(Entity entity) {
    LOG("HENLOO");
}

void Button::PointerUp(Entity entity) {
    LOG("GOODBYE");
    ECS::DestroyEntity(entity);
}
