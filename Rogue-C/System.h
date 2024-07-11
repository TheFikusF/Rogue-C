#pragma once
#include "Entity.h"
#include <memory>
#include <set>
#include <unordered_map>

class System {
public:
	Signature signature;
	std::set<Entity> Entities;
};

class SystemManager {
public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem() {
		const char* typeName = typeid(T).name();

		//assert(_systems.find(typeName) == _systems.end() && "Registering system more than once.");

		auto system = std::make_shared<T>();
		_systems.insert({ typeName, system });
		SetSignature<T>(system->signature);
		return system;
	}

	template<typename T>
	void SetSignature(Signature signature) {
		const char* typeName = typeid(T).name();

		//assert(_systems.find(typeName) != _systems.end() && "System used before registered.");

		_signatures.insert({ typeName, signature });
	}

	void EntityDestroyed(Entity entity) {
		for (auto const& pair : _systems)
		{
			auto const& system = pair.second;

			system->Entities.erase(entity);
		}
	}

	void EntitySignatureChanged(Entity entity, Signature entitySignature){
		for (auto const& pair : _systems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = _signatures[type];

			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->Entities.insert(entity);
			}
			else
			{
				system->Entities.erase(entity);
			}
		}
	}

private:
	std::unordered_map<const char*, Signature> _signatures;

	std::unordered_map<const char*, std::shared_ptr<System>> _systems;
};