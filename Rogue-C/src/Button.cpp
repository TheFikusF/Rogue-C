#include "Button.h"
#include "LOG.h"
#include "Transform.h"

Button::Button() : scale() {}

Button::Button(Vec2 scale, void (*onClickFunc)(void)) : scale(scale), onClick(onClickFunc), onUp([]() -> void {}) {}

void Button::PointerDown(Entity entity) {
    MTransform& tr = ECS::GetComponent<MTransform>(entity);
    TweenSystem::Kill(_tween);
    _tween = TweenSystem::To(tr.scale, Vec2(scale.x * 0.9f, scale.y * 1.1f), Vec2::Lerp, 0.2f);
    onClick();
}

void Button::PointerUp(Entity entity) {
    MTransform& tr = ECS::GetComponent<MTransform>(entity);
    TweenSystem::Kill(_tween);
    _tween = TweenSystem::To(tr.scale, scale, Vec2::Lerp, 0.2f);
    onUp();
}
