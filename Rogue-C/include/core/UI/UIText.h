#pragma once
#include "./include/raylib/raylib.h"
#include "./include/core/ecs/ECS.h"
#include <cstdint>
#include <string>

enum class TextAlign : std::uint8_t {
    Left,
    Center,
    Right
};

struct UIText {
    std::string text;
    Color color;
    std::uint8_t minFontSize;
    std::uint8_t maxFontSize;
    TextAlign align;

    UIText();
    UIText(std::string text, Color color);
};

class UITextSystem : public Core::System {
public:
    UITextSystem();
    void Draw() override;
};
