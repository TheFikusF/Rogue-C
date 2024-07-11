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
		size_t newIndex = _size;
		_entityToIndexMap[entity] = newIndex;
		_indexToEntityMap[newIndex] = entity;
		_components[newIndex] = component;
		++_size;
	}
	
	void RemoveComponent(Entity entity) {
		size_t indexOfRemovedEntity = _entityToIndexMap[entity];
		size_t indexOfLastElement = _size - 1;
		_components[indexOfRemovedEntity] = _components[indexOfLastElement];

		Entity entityOfLastElement = _indexToEntityMap[indexOfLastElement];
		_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		_entityToIndexMap.erase(entity);
		_indexToEntityMap.erase(indexOfLastElement);

		--_size;
	}
	
	T& GetData(Entity entity) {
		/*if (_entityToIndexMap.find(entity) == _entityToIndexMap.end()) {
			return NULL;
		}*/

		return _components[_entityToIndexMap[entity]];
	}
	
	void EntityDestroyed(Entity entity) override {
		if (_entityToIndexMap.find(entity) != _entityToIndexMap.end())
		{
			RemoveComponent(entity);
		}
	}

private:
	std::array<T, MAX_ENTITIES> _components{};
	std::unordered_map<Entity, size_t> _entityToIndexMap{};
	std::unordered_map<size_t, Entity> _indexToEntityMap{};
	size_t _size;
};
