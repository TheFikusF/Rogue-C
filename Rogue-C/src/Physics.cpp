#include "Physics.h"

PhysicsSystem::PhysicsSystem() {
	signature.set(ECS::GetComponentType<MTransform>());
	signature.set(ECS::GetComponentType<Collider2D>());
	collisions.reserve(5000);
}

void PhysicsSystem::Update(float dt) {

	UpdateVelocities(dt);
	FindCollisions();
	ResolveCollisions();

	collisions.clear();
}

void PhysicsSystem::UpdateVelocities(float dt) {
	static const float gravity = 9.81f;

	for (auto const& entity : Entities) {
		MTransform& tr = ECS::GetComponent<MTransform>(entity);
		Collider2D& collider = ECS::GetComponent<Collider2D>(entity);

		if (collider.useGravity) {
			collider.velocity.y -= gravity * dt;
		}

		tr.position += collider.velocity;
		
		if (collider.kinematic) {
			collider.velocity = Vec2();
		}
	}
}

void PhysicsSystem::FindCollisions() {
	for (auto const& entity1 : Entities) {
		const MTransform& tr1 = ECS::GetComponent<MTransform>(entity1);
		const Collider2D& collider1 = ECS::GetComponent<Collider2D>(entity1);

		for (auto const& entity2 : Entities) {
			const MTransform& tr2 = ECS::GetComponent<MTransform>(entity2);
			const Collider2D& collider2 = ECS::GetComponent<Collider2D>(entity2);

			if (entity1 == entity2) {
				break;
			}

			Collision2D collision = IsColliding(collider1, collider2, tr1, tr2);
			collision.a = entity1;
			collision.b = entity2;

			if (collision.hasCollision) {
				collisions.emplace_back(collision);
			}
		}
	}
}

void PhysicsSystem::ResolveCollisions() {
	for (Collision2D const& collision : collisions) {
		MTransform& tr1 = ECS::GetComponent<MTransform>(collision.a);
		MTransform& tr2 = ECS::GetComponent<MTransform>(collision.b);
		Collider2D& collider1 = ECS::GetComponent<Collider2D>(collision.a);
		Collider2D& collider2 = ECS::GetComponent<Collider2D>(collision.b);
		Vec2 relativeVelocity = collider1.velocity - collider2.velocity;
		float velocityAlongNormal = Vec2::Dot(relativeVelocity, collision.normal);

		if (velocityAlongNormal > 0) {
			continue;
		}

		float e = 1.0f; // coefficient of restitution (1 for elastic, 0 for inelastic)
		float j = -(1 + e) * velocityAlongNormal / (1 / collider1.mass + 1 / collider2.mass);

		Vec2 impulse = collision.normal * j;
		collider1.velocity -= impulse / collider1.mass;
		collider2.velocity += impulse / collider2.mass;

		float totalMass = collider1.mass + collider2.mass;
		Vec2 correction = collision.normal * (collision.depth / totalMass);
		tr1.position = tr1.position - correction * (collider1.mass / totalMass);
		tr2.position = tr2.position + correction * (collider2.mass / totalMass);
	}
}

Collision2D IsColliding(const Collider2D& a, const Collider2D& b, const MTransform& trA, const MTransform& trB) {
	Vec2 pointA = trA.position + trB.position - trA.position;
	Vec2 pointB = trB.position + trA.position - trB.position;
	
	return Collision2D{
		.hasCollision = Vec2::Distance(trA.position, trB.position) <= trA.scale.x + trB.scale.x,
		.a = 0,
		.b = 0,
		.pointA = pointA,
		.pointB = pointB,
		.normal = (trB.position - trA.position).GetNormalized(),
		.depth = Vec2::Distance(pointA, pointB),
	};
}
