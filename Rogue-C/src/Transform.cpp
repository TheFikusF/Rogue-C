#include "Transform.h"

const Vec2 MTransformSystem::GetRealPosition(const Entity& entity) {
	Vec2 pos = ECS::GetComponent<MTransform>(entity).position;
	//GetPosition(entity, pos);
	return pos;
}

const void MTransformSystem::GetPosition(const Entity& entity, Vec2& pos) {
	Entity parent = ECS::GetParent(entity);
	if (ECS::HasComponent<MTransform>(entity)) {
		pos += ECS::GetComponent<MTransform>(entity).position;
	}

	if (parent > MAX_ENTITIES) {
		return;
	}

	GetPosition(parent, pos);
}