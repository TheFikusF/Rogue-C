#include "./include/core/physics/Collisions.h"

template<>
void Serialization::Read<Collider2D>(const Serialization::Node* current, Collider2D& target) {
    if(current->name == "isStatic") target.isStatic = std::stoi(current->value);
    if(current->name == "isTrigger") target.isTrigger = std::stoi(current->value);
    if(current->name == "useGravity") target.useGravity = std::stoi(current->value);
    if(current->name == "kinematic") target.kinematic = std::stoi(current->value);
    if(current->name == "layer") target.layer = std::stoi(current->value);
    if(current->name == "mass") target.mass = std::stof(current->value);
}

template<>
void Serialization::Write<Collider2D>(Serialization::Node* current, const Collider2D& from) {
    current->AddChild("isStatic", std::to_string(from.isStatic));
    current->AddChild("isTrigger", std::to_string(from.isTrigger));
    current->AddChild("useGravity", std::to_string(from.useGravity));
    current->AddChild("kinematic", std::to_string(from.kinematic));
    current->AddChild("layer", std::to_string(from.layer));
    current->AddChild("mass", std::to_string(from.mass));
}

bool PointInRectangle(Vec2 point, Vec2 position, Vec2 scale) { 
    return point.x >= position.x - scale.x &&
            point.x <= position.x + scale.x &&
            point.y >= position.y - scale.y &&
            point.y <= position.y + scale.y;
}
