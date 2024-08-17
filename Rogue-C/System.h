#pragma once
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "ASSERT.h"
#include "Collisions.h"
#include "Entity.h"

namespace Core {
    namespace Debug {
		extern std::uint32_t totalUpdateTime;
		extern std::uint32_t totalPhysicsTime;
		extern std::uint32_t totalDrawTime;
		extern std::uint32_t totalSyncTime;

		extern std::unordered_map<const char*, std::uint32_t> updateTimings;
		extern std::unordered_map<const char*, std::uint32_t> physUpdateTimings;
		extern std::unordered_map<const char*, std::uint32_t> drawTimings;
		extern std::unordered_map<const char*, std::uint32_t> syncTimings;
	}

	class System {
	public:
		Signature signature;
		std::unordered_set<Entity> Entities;

		virtual void Update(float dt) {}
		virtual void PhysicsUpdate(float dt) {}
		virtual void Draw() {}

		virtual void Sync() {}

		virtual void OnEntityAdded(const Entity entity) {}
		virtual void OnEntityRemoved(const Entity entity) {}

		virtual void OnCollision(const Collision2D& collision) {}
		virtual void OnTrigger(const Collision2D& collision) {}
	};

	class SystemManager {
	public:
		template <typename T>
		std::shared_ptr<T> RegisterSystem() {
			const char* typeName = typeid(T).name();

			ASSERT(_systems.find(typeName) == _systems.end(), "Registering system more than once.");

			auto system = std::make_shared<T>();
			_systems[typeName] = system;
			return system;
		}

		template <typename T>
		std::weak_ptr<T> GetSystem() {
			const char* typeName = typeid(T).name();
			//std::shared_ptr<System> system = _systems[typeName];
			return std::static_pointer_cast<T>(_systems[typeName]);
		}

		void FinishRegistering();

		void EntityDestroyed(Entity entity);

		void EntitySignatureChanged(Entity entity, Signature entitySignature);

		void HandleCollision(const Collision2D& collision, const Signature entitySignature);

		void Update(float dt);

		void PhysicsUpdate(float dt);

		void Draw();

		void Sync();

	private:
		std::unordered_map<const char*, Signature> _signatures;

		std::unordered_map<const char*, std::shared_ptr<System>> _systems;
	};
}
