#pragma once
#include "System.h"
#include "ECS.h"
#include "Vec2.h"
#include "Transform.h"

struct Collider2D {
	bool isStatic;
	bool isTrigger;
	bool useGravity;
	bool kinematic;
	float mass;
	Vec2 force;
	Vec2 velocity;
};

struct Collision2D {
	bool hasCollision;
	Entity a;
	Entity b;
	Vec2 pointA;
	Vec2 pointB;
	Vec2 normal;
	float depth;
};

class PhysicsSystem : public System {
public:
	PhysicsSystem();
	void Update(float dt);

private:
	void UpdateVelocities(float dt);
	void FindCollisions();
    void ResolveCollisions();

    void CorrectPositions(const Collider2D& a, const Collider2D& b, const Collision2D& collision, 
		MTransform& tr1, MTransform& tr2);

    void CorrectVelocities(Collider2D& a, Collider2D& b, const Collision2D& collision);

private:
	std::vector<Collision2D> collisions;
};

Collision2D IsColliding(const Collider2D& a, const Collider2D& b, const MTransform& trA, const MTransform& trB);
