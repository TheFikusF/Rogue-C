#include "./include/core/UI/UIText.h"
#include "./include/core/Transform.h"

using namespace Core;

UIText::UIText() {}

UIText::UIText(std::string text, Color color) : text(text), color(color), minFontSize(10), maxFontSize(60), align(TextAlign::Center) {}

UITextSystem::UITextSystem() {
    signature.set(ECS::GetComponentType<UIText>());
    signature.set(ECS::GetComponentType<MTransform>());
}

void UITextSystem::Draw() {
    for(Entity const entity : Entities) {
        const MTransform& tr = ECS::GetComponent<MTransform>(entity);
        const UIText& textComponent = ECS::GetComponent<UIText>(entity);

        const Vec2 pos = MTransformSystem::GetRealPosition(entity);
        const char* text = textComponent.text.c_str();
        auto minSize = MeasureText(text, textComponent.minFontSize);
        auto maxSize = MeasureText(text, textComponent.maxFontSize);
        DrawText(text, 
            pos.x - tr.scale.x, 
            pos.y, 
            MathF::Lerp(textComponent.minFontSize, textComponent.maxFontSize, MathF::InvLerp(minSize, maxSize, tr.scale.x * 2)), 
            textComponent.color);
    }
}
