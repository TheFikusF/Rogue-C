#pragma once
#include <unordered_map>
#include <memory>

#include "./include/core/ecs/Entity.h"
#include "./include/core/ecs/ComponentArray.h"
#include "./include/core/ASSERT.h"

namespace Core {
	class ComponentManager {
	public:
		~ComponentManager();
		
		template<typename T>
		void RegisterComponent() {
			std::size_t hash = typeid(T).hash_code();
			LOG("registering type: {}, {}", std::string(typeid(T).name()), hash);
			ASSERT(_componentTypes.find(hash) == _componentTypes.end(), "Registering component type more than once.");

			_componentTypes[hash] = _nextComponentType;

			_componentArrays[hash] = GetComponentArray<T>();
			GetComponentArray<T>()->Init();
			++_nextComponentType;
		}

		template<typename T>
		ComponentType GetComponentType() {
			return _componentTypes[typeid(T).hash_code()];
		}

		ComponentType GetComponentTypeFromHash(std::size_t hash) {
			return _componentTypes[hash];
		}

		void AddComponent(Entity entity, std::size_t componentTypeHash, void* data);

		template<typename T>
		void AddComponent(Entity entity, T component) {
			GetComponentArray<T>()->AddComponent(entity, component);
		}

		template<typename T>
		void RemoveComponent(Entity entity) {
			GetComponentArray<T>()->RemoveComponent(entity);
		}

		void const* GetComponent(Entity entity, std::size_t componentTypeHash);

		template<typename T>
		T& GetComponent(Entity entity) {
			//ASSERT(_componentArrays.find(typeid(T).hash_code()) != _componentArrays.end(), "Component not registered before use.");
			return GetComponentArray<T>()->GetData(entity);
		}

		void EntityDestroyed(Entity entity);

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
}