#pragma once
#include "ComponentManager.h"
#include "Entity.h"
#include "System.h"
#include <memory>

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

	static void DestoryEntity(Entity entity) {
		_entityManager->Destroy(entity);
		_componentManager->EntityDestroyed(entity);
		_systemManager->EntityDestroyed(entity);
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
		_componentManager->RemoveComponent<T>(entity);

		auto signature = _entityManager->GetSignature(entity);
		signature.set(_componentManager->GetComponentType<T>(), false);
		_entityManager->SetSignature(entity, signature);

		_systemManager->EntitySignatureChanged(entity, signature);

	}

	template<typename T>
	static T& GetComponent(Entity entity) {
		return _componentManager->GetComponent<T>(entity);
	}

private:
	static std::unique_ptr<EntityManager> _entityManager;
	static std::unique_ptr<SystemManager> _systemManager;
	static std::unique_ptr<ComponentManager> _componentManager;
};
