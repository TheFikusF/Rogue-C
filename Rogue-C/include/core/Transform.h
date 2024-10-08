#pragma once
#include "./include/core/ecs/System.h"
#include "./include/core/ecs/ECS.h"
#include "./include/core/Vec2.h"
#include <unordered_map>
#include <vector>

struct MTransform {
	Vec2 position;
	Vec2 scale;
	float rotation;

	MTransform();
	MTransform(Vec2 position);
	MTransform(Vec2 position, Vec2 scale);
	MTransform(Vec2 position, Vec2 scale, float rotation);
};

class MTransformSystem : public Core::System {
public:
	static const Vec2 GetRealPosition(const Entity& entity);

private:
	static const void GetPosition(const Entity& entity, Vec2& pos);
};