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
	static void Init();
	
	static void Clear();

	template<typename T>
	static std::shared_ptr<T> RegisterSystem() {
		return _instance->_systemManager->RegisterSystem<T>();
	}

	template<typename T>
	static void RegisterComponent() {
		_instance->_componentManager->RegisterComponent<T>();
	}

	static void FinishRegistering();

	static Entity CreateEntity() {
        std::lock_guard<std::mutex> guard(_instance->ecsMutex);
		return _instance->_entityManager->New();
	}

	template<typename T>
	static ComponentType GetComponentType() {
		return _instance->_componentManager->GetComponentType<T>();
	}

	static void DestroyEntity(Entity entity);

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

	static Entity GetEntityCount();

	static void HandleCollisions(const std::vector<Collision2D>& collision);

	static Signature GetEntitySignature(const Entity entity);

	static Entity GetParent(const Entity entity);

	static void SetParent(const Entity& child, const Entity& parent);

	static void FreeBin();

	static void Update(float dt);

	static void PhysicsUpdate(float dt);

	static void Draw();

private:
	ECS();

	bool _finishedRegistering;

	std::mutex ecsMutex;
	
	std::unique_ptr<EntityManager> _entityManager;
	std::unique_ptr<SystemManager> _systemManager;
	std::unique_ptr<ComponentManager> _componentManager;

	std::vector<Entity> _scheduledForDeletion;
	std::unordered_map<Entity, Signature> _scheduledSignatures;
	std::unordered_map<Entity, Entity> _scheduledParents;
	std::vector<Collision2D> _collisions;

	static ECS* _instance;

	static void DestroyEntityInstantly(Entity entity);
};
