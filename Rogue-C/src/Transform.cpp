#include "Transform.h"

const Vec2 MTransformSystem::GetRealPosition(const Entity& entity) {
	//Vec2 pos = ECS::GetComponent<MTransform>(entity).position;
	Vec2 pos;
	GetPosition(entity, pos);
	return pos;
}

const void MTransformSystem::GetPosition(const Entity& entity, Vec2& pos) {
	// Entity parent = entity;
	// while(parent < MAX_ENTITIES) {
	// 	if (ECS::HasComponent<MTransform>(parent)) {
	// 		pos += ECS::GetComponent<MTransform>(parent).position;
	// 	}
	// 	parent = ECS::GetParent(parent);
	// }
	Entity parent = ECS::GetParent(entity);
	if (ECS::HasComponent<MTransform>(entity)) {
		pos += ECS::GetComponent<MTransform>(entity).position;
	}

	if (parent >= MAX_ENTITIES) {
		return;
	}

	GetPosition(parent, pos);
}