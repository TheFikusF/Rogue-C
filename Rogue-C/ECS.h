#pragma once
#include "ComponentManager.h"
#include "Entity.h"
#include "System.h"
#include "Collisions.h"
#include <memory>
#include <iostream>
#include <chrono>

class ECS
{
public:
	static void Init() {
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
		return _entityManager->New();
	}

	template<typename T>
	static ComponentType GetComponentType() {
		return _componentManager->GetComponentType<T>();
	}

	static void DestroyEntity(Entity entity) {
		_systemManager->EntityDestroyed(entity);
		_componentManager->EntityDestroyed(entity);
		_entityManager->Destroy(entity);
	}

	template<typename T>
	static void AddComponent(Entity entity, T component) {
		_componentManager->AddComponent<T>(entity, component);

		auto signature = _entityManager->GetSignature(entity);
		signature.set(_componentManager->GetComponentType<T>(), true);
		_entityManager->SetSignature(entity, signature);

		_systemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	static void RemoveComponent(Entity entity) {
		auto signature = _entityManager->GetSignature(entity);
		signature.set(_componentManager->GetComponentType<T>(), false);

		_systemManager->EntitySignatureChanged(entity, signature);
		_entityManager->SetSignature(entity, signature);
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

	static void HandleCollision(const Collision2D& collision) {
		_systemManager->HandleCollision(collision, ECS::GetEntitySignature(collision.a));
	}

	static Signature GetEntitySignature(const Entity entity) {
		return _entityManager->GetSignature(entity);
	}

	static Entity GetParent(const Entity entity) {
		return _entityManager->GetParent(entity);
	}

	static void SetParent(const Entity& child, const Entity& parent) {
		_entityManager->SetParent(child, parent);
	}

private:
	static std::unique_ptr<EntityManager> _entityManager;
	static std::unique_ptr<SystemManager> _systemManager;
	static std::unique_ptr<ComponentManager> _componentManager;
};
