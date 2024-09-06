#pragma once
#include "./include/core/ecs/Entity.h"
#include "./include/core/Vec2.h"

using Layer = std::uint8_t;

struct Collider2D {
	bool isStatic;
	bool isTrigger;
	bool useGravity;
	bool kinematic;
	Layer layer;
	float mass;
	Vec2 force;
	Vec2 velocity;

	Collider2D() : force(), velocity() { } 
	Collider2D(bool trigger, bool useGravity, float mass) 
		: isTrigger(trigger), useGravity(useGravity), layer(0), mass(mass), isStatic(false), kinematic(false), force(), velocity() { }
};

struct Collision2D {
	bool isTrigger;
	bool hasCollision;
	Entity a;
	Entity b;
	Vec2 pointA;
	Vec2 pointB;
	Vec2 normal;
	float depth;
};

bool PointInRectangle(Vec2 point, Vec2 position, Vec2 scale);