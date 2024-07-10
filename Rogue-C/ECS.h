#pragma once
#include "ComponentManager.h"
#include "Entity.h"
#include "System.h"
#include <memory>

class ECS
{
public:
	void Init() {
		_entityManager = std::make_unique<EntityManager>();
		_componentManager = std::make_unique<ComponentManager>();
		_systemManager = std::make_unique<SystemManager>();
	}

	template<typename T>
	void RegisterSystem() {
		_systemManager->RegisterSystem<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature signature) {
		_systemManager->SetSignature<T>(signature);
	}

	template<typename T>
	void RegisterComponent() {
		_componentManager->RegisterComponent<T>();
	}

	Entity CreateEntity() {
		return _entityManager->New();
	}

	void DestoryEntity(Entity entity) {
		_entityManager->Destroy(entity);
		_componentManager->EntityDestroyed(entity);
		_systemManager->EntityDestroyed(entity);
	}

	template<typename T>
	void AddComponent(Entity entity, T component) {
		_componentManager->AddComponent<T>(entity, component);
	}

	template<typename T>
	void RemoveComponent(Entity entity) {
		_componentManager->RemoveComponent<T>(entity);
	}

	template<typename T>
	T& GetComponent(Entity entity) {
		return _componentManager->GetComponent<T>(entity);
	}

	static ECS Instance;

private:
	std::unique_ptr<EntityManager> _entityManager;
	std::unique_ptr<SystemManager> _systemManager;
	std::unique_ptr<ComponentManager> _componentManager;
};