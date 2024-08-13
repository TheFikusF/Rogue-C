#pragma once
#include "Entity.h"
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include "Collisions.h"
#include "ASSERT.h"

class System {
public:
	Signature signature;
	std::unordered_set<Entity> Entities;

	virtual void Update(float dt) { }
	virtual void PhysicsUpdate(float dt) { }
	virtual void Draw() { }

	virtual void Sync() { }

	virtual void OnEntityAdded(const Entity entity) { }
	virtual void OnEntityRemoved(const Entity entity) { }
	
	virtual void OnCollision(const Collision2D& collision) { }
	virtual void OnTrigger(const Collision2D& collision) { }
};

class SystemManager {
public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem() {
		const char* typeName = typeid(T).name();

		ASSERT(_systems.find(typeName) == _systems.end(), "Registering system more than once.");

		auto system = std::make_shared<T>();
		_systems[typeName] = system;
		return system;
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

	std::unordered_map<const char*, std::uint32_t> _updateTimings;
	std::unordered_map<const char*, std::uint32_t> _physUpdateTimings;
	std::unordered_map<const char*, std::uint32_t> _drawTimings;
	std::unordered_map<const char*, std::uint32_t> _syncTimings;

	std::unordered_map<const char*, std::shared_ptr<System>> _systems;
};