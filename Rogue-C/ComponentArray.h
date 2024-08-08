#pragma once
#include <unordered_map>
#include "Entity.h"
#include "ASSERT.h"

class IComponentArray {
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
public:
	ComponentArray() {
		for(Entity i = 0; i < MAX_ENTITIES; i++) {
			_sparseToDense[i] = MAX_ENTITIES;
			_denseToSparse[i] = MAX_ENTITIES;
		}
	}

	void AddComponent(Entity entity, T component) {
		_sparseToDense[entity] = _size;
		_denseToSparse[_size] = entity;
		_components[_size] = component;
		_size++;
	}
	
	void RemoveComponent(Entity entity) {
		ASSERT(_sparseToDense[entity] != MAX_ENTITIES, "There is no such component on this entity");
		Entity indexOfRemoved = _sparseToDense[entity];
		Entity indexOfLast = _size - 1;
		Entity entityOfLast = _denseToSparse[indexOfLast];

		_components[indexOfRemoved] = _components[indexOfLast];

		_sparseToDense[entity] = MAX_ENTITIES;
		_sparseToDense[entityOfLast] = indexOfRemoved;
		
		_denseToSparse[entityOfLast] = MAX_ENTITIES;
		_denseToSparse[indexOfRemoved] = entityOfLast;
		
		_size--;
	}
	
	T& GetData(Entity entity) {
		//ASSERT(_sparseToDense[entity] != MAX_ENTITIES, "Can't get component. There is no such component on this entity.");
		return _components[_sparseToDense[entity]];
	}
	
	void EntityDestroyed(Entity entity) override {
		if(_sparseToDense[entity] >= MAX_ENTITIES) {
			return;
		}
		RemoveComponent(entity);
	}

private:
	std::array<Entity, MAX_ENTITIES> _sparseToDense{ MAX_ENTITIES };
	std::array<Entity, MAX_ENTITIES> _denseToSparse{ MAX_ENTITIES };
	std::array<T, MAX_ENTITIES> _components;
	Entity _size = 0;
};
