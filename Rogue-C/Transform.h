#pragma once
#include "Vec2.h"
#include "System.h"
#include "ECS.h"
#include <unordered_map>
#include <vector>

struct MTransform {
	Vec2 position;
	Vec2 scale;
};

class MTransformSystem : public System {
public:
	static const Vec2 GetRealPosition(const Entity& entity);

private:
	static const void GetPosition(const Entity& entity, Vec2& pos);
};