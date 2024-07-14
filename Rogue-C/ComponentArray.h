#pragma once
#include <unordered_map>
#include "Entity.h"

class IComponentArray {
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
public:
	void AddComponent(Entity entity, T component) {
		_components[entity] = component;
	}
	
	void RemoveComponent(Entity entity) {
		//_components[entity] = T();
	}
	
	T& GetData(Entity entity) {
		/*if (_entityToIndexMap.find(entity) == _entityToIndexMap.end()) {
			return NULL;
		}*/

		return _components[entity];
	}
	
	void EntityDestroyed(Entity entity) override {
		// if (_entityToIndexMap.find(entity) != _entityToIndexMap.end())
		// {
		// 	RemoveComponent(entity);
		// }
	}

private:
	//std::array<T, MAX_ENTITIES> _components{};
	T _components[MAX_ENTITIES];
	// std::unordered_map<Entity, size_t> _entityToIndexMap{};
	// std::unordered_map<size_t, Entity> _indexToEntityMap{};
	// size_t _size;
};
