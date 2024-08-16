#pragma once
#include "System.h"
#include "ECS.h"
#include "Vec2.h"
#include "Transform.h"
#include "Collisions.h"
#include <mutex>
#include <set>

namespace Physics {
	extern std::uint32_t updateGridTime;
	extern std::uint32_t findCollisionsTime;
	extern std::uint32_t lastRawDetectionTime;
	extern std::uint32_t correctTime;
	extern std::uint32_t lastIterationsCount;

	constexpr Layer MAX_LAYERS = 16;
	constexpr std::uint32_t GRID_SIZE = 100;
    constexpr float GRAVITY = 9.81f;
	
	struct Cell {
		std::size_t hash;
		Cell(Vec2 vec);

		bool operator==(const Cell other) const;
	};

	struct CellHasher {
		std::size_t operator()(const Cell& k) const;
	};

	class PhysicsSystem : public Core::System {
	public:

	public:
		PhysicsSystem();
		void SetLayer(Layer a, Layer b, bool flag);
		bool WillCollide(Layer a, Layer b);
		void PhysicsUpdate(float dt) override;

		static std::shared_ptr<PhysicsSystem> RegisterPhysics();

	private:
		void UpdateVelocities(float dt);
		void FindCollisions();
		void CheckEntity(const Entity entity2, const Entity entity1, const Collider2D& collider1, const MTransform& tr1);

		void ResolveCollisions();

		void CorrectPositions(const Collider2D& a, const Collider2D& b, const Collision2D& collision, MTransform& tr1, MTransform& tr2);

		void CorrectVelocities(Collider2D& a, Collider2D& b, const Collision2D& collision);
		
		void UpdateGrid();

		void GetNeighbors(Entity entityToCheck, Vec2 position, const Collider2D& collision, const MTransform& transform);
		void GetCell(Entity entityToCheck, Vec2 position, const Collider2D& collision, const MTransform& transform);

	private:
		std::unordered_map<Cell, std::unordered_set<Entity>, CellHasher> _grid;

		std::array<Vec2, MAX_ENTITIES> _positions;
		std::vector<Collision2D> collisions;
		std::mutex physicsMutex;
		bool layers[MAX_LAYERS][MAX_LAYERS];

	};

	Collision2D IsColliding(Vec2 posA, Vec2 posB, Vec2 scaleA, Vec2 scaleB);
}

