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
	ECS() {
		_instance = this;
		_scheduledForDeletion.reserve(MAX_ENTITIES);
		_entityManager = std::make_unique<EntityManager>();
		_componentManager = std::make_unique<ComponentManager>();
		_systemManager = std::make_unique<SystemManager>();
	}

	template<typename T>
	static std::shared_ptr<T> RegisterSystem() {
		return _instance->_systemManager->RegisterSystem<T>();
	}

	template<typename T>
	static void SetSystemSignature(Signature signature) {
		_instance->_systemManager->SetSignature<T>(signature);
	}

	template<typename T>
	static void RegisterComponent() {
		_instance->_componentManager->RegisterComponent<T>();
	}

	static Entity CreateEntity() {
        std::lock_guard<std::mutex> guard(_instance->ecsMutex);
		return _instance->_entityManager->New();
	}

	template<typename T>
	static ComponentType GetComponentType() {
		return _instance->_componentManager->GetComponentType<T>();
	}

	static void DestroyEntity(Entity entity) {
		_instance->_scheduledForDeletion.emplace_back(entity);
	}

	template<typename T>
	static void AddComponent(Entity entity, T component) {
        std::lock_guard<std::mutex> guard(_instance->ecsMutex);
		if(_instance->_scheduledSignatures.contains(entity) == false) {
			_instance->_scheduledSignatures[entity] = _instance->_entityManager->GetSignature(entity);
		}
		_instance->_scheduledSignatures[entity].set(_instance->_componentManager->GetComponentType<T>(), true);
		_instance->_componentManager->AddComponent<T>(entity, component);
	}

	template<typename T>
	static void RemoveComponent(Entity entity) {
        std::lock_guard<std::mutex> guard(_instance->ecsMutex);
		if(_instance->_scheduledSignatures.contains(entity) == false) {
			_instance->_scheduledSignatures[entity] = _instance->_entityManager->GetSignature(entity);
		}
		_instance->_scheduledSignatures[entity].set(_instance->_componentManager->GetComponentType<T>(), false);
		_instance->_componentManager->RemoveComponent<T>(entity);
	}

	template<typename T>
	static T& GetComponent(Entity entity) {
		return _instance->_componentManager->GetComponent<T>(entity);
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
		return _instance->_entityManager->GetSignature(entity).test(_instance->_componentManager->GetComponentType<T>());
	}

	static Entity GetEntityCount() {
		return _instance->_entityManager->_entityCount;
	}

	static void HandleCollisions(const std::vector<Collision2D>& collision) {
		_instance->_collisions.assign(collision.begin(), collision.end());
	}

	static Signature GetEntitySignature(const Entity entity) {
		return _instance->_entityManager->GetSignature(entity);
	}

	static Entity GetParent(const Entity entity) {
		return _instance->_entityManager->GetParent(entity);
	}

	static void SetParent(const Entity& child, const Entity& parent) {
		_instance->_scheduledParents[child] = parent;
	}

	static void FreeBin() {
		for(auto const& collision : _instance->_collisions) {
			_instance->_systemManager->HandleCollision(collision, ECS::GetEntitySignature(collision.a));
			_instance->_systemManager->HandleCollision(Collision2D{
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

        std::lock_guard<std::mutex> guard(_instance->ecsMutex);
		for(auto const& pair : _instance->_scheduledParents) {
			_instance->_entityManager->SetParent(pair.first, pair.second);
		}
		_instance->_scheduledParents.clear();

		for(auto const& pair : _instance->_scheduledSignatures) {
			_instance->_entityManager->SetSignature(pair.first, pair.second);
			_instance->_systemManager->EntitySignatureChanged(pair.first, pair.second);
		}
		_instance->_scheduledSignatures.clear();

		for(int i = _instance->_scheduledForDeletion.size() - 1; i >= 0; i--) {
			DestroyEntityInstantly(_instance->_scheduledForDeletion[i]);
		}
		_instance->_scheduledForDeletion.clear();
	}

	static void Update(float dt) {
		_instance->_systemManager->Update(dt);
	}

	static void PhysicsUpdate(float dt) {
		_instance->_systemManager->PhysicsUpdate(dt);
	}

	static void Draw() {
		_instance->_systemManager->Draw();
	}

private:
	std::mutex ecsMutex;
	
	std::unique_ptr<EntityManager> _entityManager;
	std::unique_ptr<SystemManager> _systemManager;
	std::unique_ptr<ComponentManager> _componentManager;

	std::vector<Entity> _scheduledForDeletion;
	std::unordered_map<Entity, Signature> _scheduledSignatures;
	std::unordered_map<Entity, Entity> _scheduledParents;
	std::vector<Collision2D> _collisions;

	static ECS* _instance;

	static void DestroyEntityInstantly(Entity entity) {
		for(auto const& child : _instance->_entityManager->GetChildren(entity)) {
			DestroyEntityInstantly(child);
		}

		_instance->_systemManager->EntityDestroyed(entity);
		_instance->_componentManager->EntityDestroyed(entity);
		_instance->_entityManager->Destroy(entity);
	}
};
