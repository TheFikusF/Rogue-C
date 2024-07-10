#include "ComponentManager.h"

template<typename T>
void ComponentManager::RegisterComponent() {
	const char* typeName = typeid(T).name();

	assert(_componentTypes.find(typeName) == _componentTypes.end() && "Registering component type more than once.");

	_componentTypes.insert({ typeName, _nextComponentType });

	_componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

	++_nextComponentType;
}

template<typename T>
void ComponentManager::AddComponent(Entity entity, T component) {
	GetComponentArray<T>()->InsertData(entity, component);
}

template<typename T>
void ComponentManager::RemoveComponent(Entity entity) {
	GetComponentArray<T>()->RemoveData(entity);
}

template<typename T>
T& ComponentManager::GetComponent(Entity entity) {
	return GetComponentArray<T>()->GetData(entity);
}

template<typename T>
std::shared_ptr<ComponentArray<T>> ComponentManager::GetComponentArray() {
	const char* typeName = typeid(T).name();

	assert(_componentTypes.find(typeName) != _componentTypes.end() && "Component not registered before use.");

	return std::static_pointer_cast<ComponentArray<T>>(_componentArrays[typeName]);
}

void ComponentManager::EntityDestroyed(Entity entity) {
	for (auto const& pair : _componentArrays)
	{
		auto const& component = pair.second;

		component->EntityDestroyed(entity);
	}
}
