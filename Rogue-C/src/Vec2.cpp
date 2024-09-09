#include "./include/core/Vec2.h"

template<>
void Serialization::Read<Vec2>(const Node* current, Vec2& target) {
    if (current->name.compare("x") == 0) target.x = std::stof(current->value);
    if (current->name.compare("y") == 0) target.y = std::stof(current->value);
}

template<>
void Serialization::Write<Vec2>(Serialization::Node* parent, const Vec2& from) {
    parent->AddChild(from.x, "x");
    parent->AddChild(from.y, "y");
}