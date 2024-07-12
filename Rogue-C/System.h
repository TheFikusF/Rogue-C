#pragma once
#include "Entity.h"
#include <memory>
#include <set>
#include <unordered_map>
#include "Collisions.h"
#include <chrono>
#include <iostream>

class System {
public:
	Signature signature;
	std::set<Entity> Entities;

	virtual void OnCollision(const Collision2D& collision) { }
	virtual void OnTrigger(const Collision2D& collision) { }
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
		for (auto const& pair : _systems) {
			auto const& system = pair.second;

			system->Entities.erase(entity);
		}
	}

	void EntitySignatureChanged(Entity entity, Signature entitySignature){
		for (auto const& pair : _systems) {
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = _signatures[type];

			if ((entitySignature & systemSignature) == systemSignature) {
				system->Entities.insert(entity);
			} else {
				system->Entities.erase(entity);
			}
		}
	}

	void HandleCollision(const Collision2D& collision, const Signature entitySignature) {
		for (auto const& pair : _systems) {
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = _signatures[type];

			if ((entitySignature & systemSignature) != systemSignature) {
				continue;
			}

			//std::cout << std::chrono::system_clock::now() << " Handling collision " << type << " is trigger: " << collision.isTrigger << std::endl;
			if(collision.isTrigger) {
				//std::cout << std::chrono::system_clock::now() << " Handling collision " << type << " is trigger: " << collision.isTrigger << std::endl;
				system->OnTrigger(collision);
			} else {
				system->OnCollision(collision);
			}
		}
	}

private:
	std::unordered_map<const char*, Signature> _signatures;

	std::unordered_map<const char*, std::shared_ptr<System>> _systems;
};