#include "Physics.h"

PhysicsSystem::PhysicsSystem() {
	signature.set(ECS::GetComponentType<MTransform>());
	signature.set(ECS::GetComponentType<Collider2D>());
	collisions.reserve(5000);
}

void PhysicsSystem::Update(float dt) {

    auto findClock = std::chrono::high_resolution_clock::now();
	FindCollisions();
    auto resolveClock = std::chrono::high_resolution_clock::now();
	ResolveCollisions();
    auto updateClock = std::chrono::high_resolution_clock::now();
	UpdateVelocities(dt);
    auto endClock = std::chrono::high_resolution_clock::now();

	findTime = (resolveClock - findClock).count();
	resolveTime = (updateClock - resolveClock).count();
	correctTime = (endClock - updateClock).count();

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

		tr.position += collider.velocity * dt;
		
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

			// if(collider1.layer == collider2.layer) {
			// 	continue;
			// }

			if(collider1.isStatic == true && collider2.isStatic == true) {
				continue;
			}

			Collision2D collision = IsColliding(entity1, entity2, tr1, tr2);
			collision.isTrigger = collider1.isTrigger || collider2.isTrigger;

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

		ECS::HandleCollision(collision);
		ECS::HandleCollision(Collision2D {
			.isTrigger = collision.isTrigger,
			.hasCollision = true,
			.a = collision.b,
			.b = collision.a,
			.pointA = collision.pointB,
			.pointB = collision.pointA,
			.normal = collision.normal,
			.depth = collision.depth,
		});
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

Collision2D IsColliding(const Entity& a, const Entity& b, const MTransform& trA, const MTransform& trB) {
	Vec2 posA = MTransformSystem::GetRealPosition(a);
	Vec2 posB = MTransformSystem::GetRealPosition(b);
	Vec2 pointA = posA + posB - posA;
	Vec2 pointB = posB + posA - posB;
	
	return Collision2D{
		.hasCollision = Vec2::Distance(posA, posB) <= trA.scale.x + trB.scale.x,
		.a = a,
		.b = b,
		.pointA = pointA,
		.pointB = pointB,
		.normal = (posB - posA).GetNormalized(),
		.depth = Vec2::Distance(pointA, pointB),
	};
}
