#pragma once
#include "System.h"
#include "ECS.h"
#include "Vec2.h"
#include "Transform.h"
#include "Collisions.h"
#include <mutex>

class PhysicsSystem : public System {
public:
	static const Layer MAX_LAYERS = 16;

	std::uint32_t findTime;
	std::uint32_t resolveTime;
	std::uint32_t correctTime;

public:
	PhysicsSystem();
	void SetLayer(Layer a, Layer b, bool flag);
	bool WillCollide(Layer a, Layer b);
	void PhysicsUpdate(float dt) override;

private:
	void UpdateVelocities(float dt);
	void FindCollisions();
    void ResolveCollisions();

    void CorrectPositions(const Collider2D& a, const Collider2D& b, const Collision2D& collision, 
		MTransform& tr1, MTransform& tr2);

    void CorrectVelocities(Collider2D& a, Collider2D& b, const Collision2D& collision);

private:
	std::vector<Collision2D> collisions;
	std::mutex physicsMutex;
	bool layers[MAX_LAYERS][MAX_LAYERS];
};

Collision2D IsColliding(const Entity& a, const Entity& b, const MTransform& trA, const MTransform& trB);
