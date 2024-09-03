#include "./include/core/physics/Collisions.h"

void Collider2D::Read(std::string name, std::string value, const Serialization::Node* current) {
    if(name == "isStatic") isStatic = std::stoi(value);
    if(name == "isTrigger") isTrigger = std::stoi(value);
    if(name == "useGravity") useGravity = std::stoi(value);
    if(name == "kinematic") kinematic = std::stoi(value);
    if(name == "layer") layer = std::stoi(value);
    if(name == "mass") mass = std::stof(value);
    force = Vec2();
    velocity = Vec2();
}

void Collider2D::Write(Serialization::Node* current) {

}

bool PointInRectangle(Vec2 point, Vec2 position, Vec2 scale) { 
    return point.x >= position.x - scale.x &&
            point.x <= position.x + scale.x &&
            point.y >= position.y - scale.y &&
            point.y <= position.y + scale.y;
}
