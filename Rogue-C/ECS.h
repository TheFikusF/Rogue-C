#pragma once
#include "ComponentManager.h"
#include "Entity.h"
#include "System.h"
#include "Collisions.h"
#include <memory>
#include <iostream>
#include <chrono>
#include <mutex>

class ECS
{
public:
	static void Init() {
		_scheduledForDeletion.reserve(MAX_ENTITIES);
		_entityManager = std::make_unique<EntityManager>();
		_componentManager = std::make_unique<ComponentManager>();
		_systemManager = std::make_unique<SystemManager>();
	}

	template<typename T>
	static std::shared_ptr<T> RegisterSystem() {
		return _systemManager->RegisterSystem<T>();
	}

	template<typename T>
	static void SetSystemSignature(Signature signature) {
		_systemManager->SetSignature<T>(signature);
	}

	template<typename T>
	static void RegisterComponent() {
		_componentManager->RegisterComponent<T>();
	}

	static Entity CreateEntity() {
        std::lock_guard<std::mutex> guard(ecsMutex);
		return _entityManager->New();
	}

	template<typename T>
	static ComponentType GetComponentType() {
		return _componentManager->GetComponentType<T>();
	}

	static void DestroyEntity(Entity entity) {
		_scheduledForDeletion.emplace_back(entity);
	}

	template<typename T>
	static void AddComponent(Entity entity, T component) {
        std::lock_guard<std::mutex> guard(ecsMutex);
		if(_scheduledSignatures.contains(entity) == false) {
			_scheduledSignatures[entity] = _entityManager->GetSignature(entity);
		}
		_scheduledSignatures[entity].set(_componentManager->GetComponentType<T>(), true);
		_componentManager->AddComponent<T>(entity, component);
	}

	template<typename T>
	static void RemoveComponent(Entity entity) {
        std::lock_guard<std::mutex> guard(ecsMutex);
		if(_scheduledSignatures.contains(entity) == false) {
			_scheduledSignatures[entity] = _entityManager->GetSignature(entity);
		}
		_scheduledSignatures[entity].set(_componentManager->GetComponentType<T>(), false);
		_componentManager->RemoveComponent<T>(entity);
	}

	template<typename T>
	static T& GetComponent(Entity entity) {
		return _componentManager->GetComponent<T>(entity);
	}

	template<typename T>
	static bool TryGetComponent(Entity entity, T* component) {
		if(HasComponent<T>(entity)) {
			component = &GetComponent<T>(entity);
			return true;
		}
		return false;
	}

	template<typename T>
	static bool HasComponent(Entity entity) {
		return _entityManager->GetSignature(entity).test(_componentManager->GetComponentType<T>());
	}

	static Entity GetEntityCount() {
		return _entityManager->_entityCount;
	}

	static void HandleCollisions(const std::vector<Collision2D>& collision) {
		_collisions.assign(collision.begin(), collision.end());
	}

	static Signature GetEntitySignature(const Entity entity) {
		return _entityManager->GetSignature(entity);
	}

	static Entity GetParent(const Entity entity) {
		return _entityManager->GetParent(entity);
	}

	static void SetParent(const Entity& child, const Entity& parent) {
		_scheduledParents[child] = parent;
	}

	static void FreeBin() {
        std::lock_guard<std::mutex> guard(ecsMutex);

		for(auto const& collision : _collisions) {
			_systemManager->HandleCollision(collision, ECS::GetEntitySignature(collision.a));
			_systemManager->HandleCollision(Collision2D{
				.isTrigger = collision.isTrigger,
				.hasCollision = collision.hasCollision,
				.a = collision.b,
				.b = collision.a,
				.pointA = collision.pointB,
				.pointB = collision.pointA,
				.normal = collision.normal,
				.depth = collision.depth,

			}, ECS::GetEntitySignature(collision.b));
		}

		for(auto const& pair : _scheduledParents) {
			_entityManager->SetParent(pair.first, pair.second);
		}
		_scheduledParents.clear();

		for(auto const& pair : _scheduledSignatures) {
			_entityManager->SetSignature(pair.first, pair.second);
			_systemManager->EntitySignatureChanged(pair.first, pair.second);
		}
		_scheduledSignatures.clear();

		for(int i = _scheduledForDeletion.size() - 1; i >= 0; i--) {
			_systemManager->EntityDestroyed(_scheduledForDeletion[i]);
			_componentManager->EntityDestroyed(_scheduledForDeletion[i]);
			_entityManager->Destroy(_scheduledForDeletion[i]);
		}
		_scheduledForDeletion.clear();
	}

private:
	static std::mutex ecsMutex;
	
	static std::unique_ptr<EntityManager> _entityManager;
	static std::unique_ptr<SystemManager> _systemManager;
	static std::unique_ptr<ComponentManager> _componentManager;

	static std::vector<Entity> _scheduledForDeletion;
	static std::unordered_map<Entity, Signature> _scheduledSignatures;
	static std::unordered_map<Entity, Entity> _scheduledParents;
	static std::vector<Collision2D> _collisions;
};
