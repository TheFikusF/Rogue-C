#include "./include/core/Gradient.h"
#include "./include/core/LOG.h"
#include "./include/raylib/raymath.h"

GradientKey::GradientKey(Color color, float time) : color(color), time(time) { }

void GradientKey::Read(const Serialization::Node* current) {
    if (current->name.compare("color") == 0) color = current->Read<Rendering::SerializedColor>().ToRLColor();
    if (current->name.compare("time") == 0) time = std::stof(current->value);
}

void GradientKey::Write(Serialization::Node* current) {
    Rendering::SerializedColor(color).Write(current->AddChild("color"));
    current->AddChild("time", std::to_string(time));
}

Gradient::Gradient(std::vector<GradientKey> keyFrames) : keyFrames(keyFrames) { }

Color Gradient::Evaluate(float t) const {
    t = std::max(0.0f, std::min(1.0f, t));

    if (t <= keyFrames.front().time)
    {
        return keyFrames.front().color;
    }

    if (t >= keyFrames.back().time)
    {
        return keyFrames.back().color;
    }

    for (std::uint8_t i = 1; i < keyFrames.size() && t < keyFrames[i].time; i++) {
        t = (t - keyFrames[i - 1].time) / (keyFrames[i].time - keyFrames[i - 1].time);
        return ColorLerp(keyFrames[i - 1].color, keyFrames[i].color, t);
    }

    return keyFrames.back().color;
}

void Gradient::Read(const Serialization::Node* current) {
    if (current->name.compare("keyFrames") == 0) current->ReadVector<GradientKey>(keyFrames);
}

void Gradient::Write(Serialization::Node* current) {
    auto node = current->AddChild("keyFrames");
    node->isArray = true;
    for (auto frame : keyFrames) {
        frame.Write(node->AddChild());
    }
}

Color ColorLerp(Color start, Color end, float t) { 
    return Color(Lerp(start.r, end.r, t), Lerp(start.g, end.g, t), Lerp(start.b, end.b, t), Lerp(start.a, end.a, t)); 
}