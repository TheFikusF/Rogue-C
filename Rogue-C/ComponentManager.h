#pragma once
#include "ComponentArray.h"
#include <unordered_map>
#include "Entity.h"
#include <memory>
#include "LOG.h"

class ComponentManager {
public:
	template<typename T>
	void RegisterComponent() {
		const char* typeName = typeid(T).name();

		//assert(_componentTypes.find(typeName) == _componentTypes.end() && "Registering component type more than once.");

		_componentTypes.insert({ typeName, _nextComponentType });

		_componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

		++_nextComponentType;
	}

	template<typename T>
	ComponentType GetComponentType() {
		const char* typeName = typeid(T).name();
		return _componentTypes[typeName];
	}


	template<typename T>
	void AddComponent(Entity entity, T component) {
		//LOG(std::format("adding component {}", typeid(T).name()));
		GetComponentArray<T>()->AddComponent(entity, component);
	}

	template<typename T>
	void RemoveComponent(Entity entity) {
		//GetComponentArray<T>()->RemoveComponent(entity);
	}

	template<typename T>
	T& GetComponent(Entity entity) {
		return GetComponentArray<T>()->GetData(entity);
	}

	void EntityDestroyed(Entity entity) {
		// for (auto const& pair : _componentArrays)
		// {
		// 	auto const& component = pair.second;

		// 	component->EntityDestroyed(entity);
		// }
	}

private:
	std::unordered_map<const char*, ComponentType> _componentTypes{};

	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> _componentArrays{};

	ComponentType _nextComponentType{};

	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray() {
		const char* typeName = typeid(T).name();

		//assert(_componentTypes.find(typeName) != _componentTypes.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(_componentArrays[typeName]);
	}
};
