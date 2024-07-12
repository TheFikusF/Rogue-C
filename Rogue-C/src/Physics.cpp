#include "Physics.h"

PhysicsSystem::PhysicsSystem() {
	signature.set(ECS::GetComponentType<MTransform>());
	signature.set(ECS::GetComponentType<Collider2D>());
	collisions.reserve(5000);
}

void PhysicsSystem::Update(float dt) {

	FindCollisions();
	ResolveCollisions();
	
	UpdateVelocities(dt);

	collisions.clear();
}

void PhysicsSystem::UpdateVelocities(float dt) {
	static const float gravity = 9.81f;

	for (auto const& entity : Entities) {
		MTransform& tr = ECS::GetComponent<MTransform>(entity);
		Collider2D& collider = ECS::GetComponent<Collider2D>(entity);

		if(collider.isStatic) {
			continue;
		}

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

			if(collider1.isStatic == true && collider2.isStatic == true) {
				continue;
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
		Collider2D& collider1 = ECS::GetComponent<Collider2D>(collision.a);
		Collider2D& collider2 = ECS::GetComponent<Collider2D>(collision.b);

		if(collider1.isTrigger == true || collider2.isTrigger == true) {
			continue;
		}

		MTransform& tr1 = ECS::GetComponent<MTransform>(collision.a);
		MTransform& tr2 = ECS::GetComponent<MTransform>(collision.b);

        CorrectPositions(collider1, collider2, collision, tr1, tr2);

        CorrectVelocities(collider1, collider2, collision);
	}
}

void PhysicsSystem::CorrectPositions(const Collider2D& a, const Collider2D& b, 
	const Collision2D& collision, MTransform& tr1, MTransform& tr2) {
    float totalMass = a.mass + b.mass;
    Vec2 correction = collision.normal * (collision.depth / totalMass);
    tr1.position = tr1.position - correction * (a.mass / totalMass);
    tr2.position = tr2.position + correction * (b.mass / totalMass);
}

void PhysicsSystem::CorrectVelocities(Collider2D& a, Collider2D& b, const Collision2D& collision) {
	Vec2 relativeVelocity = a.velocity - b.velocity;
	float velocityAlongNormal = Vec2::Dot(relativeVelocity, collision.normal);

	if (velocityAlongNormal > 0) {
		return;
	}

	float e = 1.0f; // coefficient of restitution (1 for elastic, 0 for inelastic)
	float j = -(1 + e) * velocityAlongNormal / (1 / a.mass + 1 / b.mass);

	Vec2 impulse = collision.normal * j;
	a.velocity -= impulse / a.mass;
	a.velocity += impulse / b.mass;
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
