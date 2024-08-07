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
		std::lock_guard<std::mutex> lock(_mutex);
		_sparseToDense[entity] = _size;
		_denseToSparse[_size] = entity;
		_components[_size] = component;
		_size++;
	}
	
	void RemoveComponent(Entity entity) {
		std::lock_guard<std::mutex> lock(_mutex);
		Entity indexOfRemoved = _sparseToDense[entity];
		Entity indexOfLast = _size - 1;

		_components[indexOfRemoved] = _components[indexOfLast];

		_sparseToDense[_denseToSparse[indexOfLast]] = indexOfRemoved;
		_denseToSparse[indexOfRemoved] = _denseToSparse[indexOfLast];
		_size--;
	}
	
	T& GetData(Entity entity) {
		return _components[_sparseToDense[entity]];
	}
	
	void EntityDestroyed(Entity entity) override {
		RemoveComponent(entity);
	}

private:
	std::mutex _mutex;
	std::array<Entity, MAX_ENTITIES> _sparseToDense{ 0 };
	std::array<Entity, MAX_ENTITIES> _denseToSparse{ 0 };
	std::array<T, MAX_ENTITIES> _components;
	Entity _size = 0;
};
