#pragma once
#include "ComponentArray.h"
#include <unordered_map>
#include "Entity.h"
#include <memory>
#include "ASSERT.h"

class ComponentManager {
public:
	template<typename T>
	void RegisterComponent() {
		std::size_t hash = typeid(T).hash_code();
		LOG(std::format("registering type: {}, {}", std::string(typeid(T).name()), hash));
		ASSERT(_componentTypes.find(hash) == _componentTypes.end(), "Registering component type more than once.");

		_componentTypes[hash] = _nextComponentType;

		_componentArrays[hash] = GetComponentArray<T>();
		++_nextComponentType;
	}

	template<typename T>
	constexpr ComponentType GetComponentType() {
		return _componentTypes[typeid(T).hash_code()];
	}


	template<typename T>
	void AddComponent(Entity entity, T component) {
		GetComponentArray<T>()->AddComponent(entity, component);
	}

	template<typename T>
	void RemoveComponent(Entity entity) {
		GetComponentArray<T>()->RemoveComponent(entity);
	}

	template<typename T>
	constexpr T& GetComponent(Entity entity) {
		//ASSERT(_componentArrays.find(typeid(T).hash_code()) != _componentArrays.end(), "Component not registered before use.");
		return GetComponentArray<T>()->GetData(entity);
	}

	void EntityDestroyed(Entity entity) {
		for (auto const& pair : _componentArrays)
		{
			auto const& component = pair.second;
			component->EntityDestroyed(entity);
		}
	}

private:
	std::unordered_map<std::size_t, ComponentType> _componentTypes{};

	std::unordered_map<std::size_t, std::shared_ptr<IComponentArray>> _componentArrays{};

	ComponentType _nextComponentType{};

	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray() {
		static std::shared_ptr<ComponentArray<T>> _array = std::make_shared<ComponentArray<T>>();
		return _array;
	}
};
