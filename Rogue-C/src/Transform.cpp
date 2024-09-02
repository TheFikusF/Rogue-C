#include "./include/core/Transform.h"

using namespace Core;

MTransform::MTransform() 
	: position(Vec2()), scale(Vec2()), rotation(0) {}

MTransform::MTransform(Vec2 position) 
	: position(position), scale(Vec2()), rotation(0) {}

MTransform::MTransform(Vec2 position, Vec2 scale) 
	: position(position), scale(scale), rotation(0) {}

MTransform::MTransform(Vec2 position, Vec2 scale, float rotation)
    : position(position), scale(scale), rotation(rotation) {}

const Vec2 MTransformSystem::GetRealPosition(
    const Entity entity) {
    Vec2 pos;
    GetPosition(entity, pos);
    return pos;
}

const void MTransformSystem::GetPosition(const Entity entity, Vec2& pos) {
	Entity parent = ECS::GetParent(entity);
	if (ECS::HasComponent<MTransform>(entity)) {
		pos += ECS::GetComponent<MTransform>(entity).position;
	}

	if (parent >= MAX_ENTITIES) {
		return;
	}

	GetPosition(parent, pos);
}

void MTransform::Read(std::string name, std::string value, const Serialization::Node* current) {
	if(name.compare("position") == 0) position = current->Read<Vec2>(); 
	if(name.compare("scale") == 0) scale = current->Read<Vec2>(); 
	if(name.compare("rotation") == 0) rotation = std::stof(value); 
}

void MTransform::Write(Serialization::Node* node) { 
	position.Write(node->AddChild("position"));
	scale.Write(node->AddChild("scale"));
	node->AddChild("rotation", std::to_string(rotation));
}