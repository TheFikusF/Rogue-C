#pragma once
#include "Entity.h"
#include <memory>
#include <set>
#include <unordered_map>

class System {
public:
	std::set<Entity> Entities;
};

class SystemManager {
public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem();

	template<typename T>
	void SetSignature(Signature signature);

	void EntityDestroyed(Entity entity);

	void EntitySignatureChanged(Entity entity, Signature entitySignature);

private:
	std::unordered_map<const char*, Signature> _signatures;

	std::unordered_map<const char*, std::shared_ptr<System>> _systems;
};