#pragma once 
#include "./include/core/rendering/Drawer.h"
#include "./include/core/serialization/Serializable.h"
#include "./include/raylib/raylib.h"
#include <vector>
#include <cstdint>

struct GradientKey : public Serialization::Serializable {
    Color color;
    float time;

    GradientKey() = default;
    GradientKey(Color color, float time);

    void Read(const Serialization::Node* current) override;
    void Write(Serialization::Node* current) override;
};


struct Gradient : public Serialization::Serializable {
    std::vector<GradientKey> keyFrames;

    Gradient() : keyFrames() { }
    Gradient(std::vector<GradientKey> keyFrames);

    Color Evaluate(float t) const;

    void Read(const Serialization::Node* current) override;
    void Write(Serialization::Node* current) override;
};

Color ColorLerp(Color start, Color end, float t);