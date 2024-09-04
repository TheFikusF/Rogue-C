#include "./include/core/physics/Collisions.h"

void Collider2D::Read(const Serialization::Node* current) {
    if(current->name == "isStatic") isStatic = std::stoi(current->value);
    if(current->name == "isTrigger") isTrigger = std::stoi(current->value);
    if(current->name == "useGravity") useGravity = std::stoi(current->value);
    if(current->name == "kinematic") kinematic = std::stoi(current->value);
    if(current->name == "layer") layer = std::stoi(current->value);
    if(current->name == "mass") mass = std::stof(current->value);
}

void Collider2D::Write(Serialization::Node* current) {
    current->AddChild("isStatic", std::to_string(isStatic));
    current->AddChild("isTrigger", std::to_string(isTrigger));
    current->AddChild("useGravity", std::to_string(useGravity));
    current->AddChild("kinematic", std::to_string(kinematic));
    current->AddChild("layer", std::to_string(layer));
    current->AddChild("mass", std::to_string(mass));
}

bool PointInRectangle(Vec2 point, Vec2 position, Vec2 scale) { 
    return point.x >= position.x - scale.x &&
            point.x <= position.x + scale.x &&
            point.y >= position.y - scale.y &&
            point.y <= position.y + scale.y;
}
