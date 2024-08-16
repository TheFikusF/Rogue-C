#pragma once
#include "Vec2.h"
#include "System.h"
#include "ECS.h"
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