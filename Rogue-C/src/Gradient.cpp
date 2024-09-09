#include "./include/core/Gradient.h"
#include "./include/core/LOG.h"
#include "./include/raylib/raymath.h"

GradientKey::GradientKey(Color color, float time) : color(color), time(time) { }

template<>
void Serialization::Read<GradientKey>(const Serialization::Node* current, GradientKey& target) {
    if (current->name.compare("color") == 0) target.color = current->Read<Color>();
    if (current->name.compare("time") == 0) target.time = std::stof(current->value);
}

template<>
void Serialization::Write<GradientKey>(Serialization::Node* current, const GradientKey& from) {
    current->AddChild(from.color, "color");
    current->AddChild(from.time, "time");
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

template<>
void Serialization::Read<Gradient>(const Serialization::Node* current, Gradient& target) {
    if (current->name.compare("keyFrames") == 0) current->ReadVector<GradientKey>(target.keyFrames);
}

template<>
void Serialization::Write<Gradient>(Serialization::Node* current, const Gradient& from) {
    auto node = current->AddChild("keyFrames");
    node->isArray = true;
    for (auto frame : from.keyFrames) {
        node->AddChild(frame);
    }
}

Color ColorLerp(Color start, Color end, float t) { 
    return Color(Lerp(start.r, end.r, t), Lerp(start.g, end.g, t), Lerp(start.b, end.b, t), Lerp(start.a, end.a, t)); 
}