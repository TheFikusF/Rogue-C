#pragma once 
#include "./include/raylib/raylib.h"
#include <vector>
#include <cstdint>

struct GradientKey
{
    Color color;
    float time;

    GradientKey() : color(BLACK), time(0) {}
    GradientKey(Color color, float time) : color(color), time(time) {}
};


struct Gradient
{
    std::vector<GradientKey> keyFrames;

    Color Evaluate(float t) const;
};

Color ColorLerp(Color start, Color end, float t);