#pragma once
#include "Vec2.h"
#include "Entity.h"

struct Collider2D {
	bool isStatic;
	bool isTrigger;
	bool useGravity;
	bool kinematic;
	//std::uint8_t layer;
	float mass;
	Vec2 force;
	Vec2 velocity;

	Collider2D() = default; 
	Collider2D(bool trigger, bool useGravity, float mass) 
		: isTrigger(trigger), useGravity(useGravity), mass(mass), isStatic(false), kinematic(false), force(), velocity() {}
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