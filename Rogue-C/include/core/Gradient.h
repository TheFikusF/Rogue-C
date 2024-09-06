#pragma once 
#include "./include/core/rendering/Drawer.h"
#include "./include/core/serialization/Serializable.h"
#include "./include/raylib/raylib.h"
#include <vector>
#include <cstdint>

struct GradientKey {
    Color color;
    float time;

    GradientKey() = default;
    GradientKey(Color color, float time);
};


struct Gradient {
    std::vector<GradientKey> keyFrames;

    Gradient() : keyFrames() { }
    Gradient(std::vector<GradientKey> keyFrames);

    Color Evaluate(float t) const;
};

Color ColorLerp(Color start, Color end, float t);