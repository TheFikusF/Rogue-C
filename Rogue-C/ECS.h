#pragma once
#include "ComponentManager.h"
#include "Entity.h"
#include "System.h"
#include <memory>

class ECS
{
public:
	void Init();

	template<typename T>
	void RegisterSystem();

	template<typename T>
	void SetSystemSignature(Signature signature);

	template<typename T>
	void RegisterComponent();

	Entity CreateEntity();
	void DestoryEntity(Entity entity);

	template<typename T>
	void AddComponent(Entity entity, T component);

	template<typename T>
	void RemoveComponent(Entity entity);

	template<typename T>
	T& GetComponent(Entity entity);

	static ECS Instance;

private:
	std::unique_ptr<EntityManager> _entityManager;
	std::unique_ptr<SystemManager> _systemManager;
	std::unique_ptr<ComponentManager> _componentManager;
};