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
		// for(Entity i = 0; i < MAX_ENTITIES; i++) {
		// 	_sparseToDense[i] = MAX_ENTITIES;
		// 	_denseToSparse[i] = MAX_ENTITIES;
		// }
		_components.reserve(MAX_ENTITIES);
	}

	void AddComponent(Entity entity, T component) {
		_sparseToDense[entity] = _size;
		_denseToSparse[_size] = entity;
		if(_size >= _components.size())
		{
			_components.push_back(component);
		}
		else
		{
			_components[_size] = component;
		}
		_size++;
	}
	
	void RemoveComponent(Entity entity) {
		ASSERT(_sparseToDense.contains(entity), "There is no such component on this entity");
		Entity indexOfRemoved = _sparseToDense[entity];
		Entity indexOfLast = _size - 1;
		Entity entityOfLast = _denseToSparse[indexOfLast];

		_components[indexOfRemoved] = _components[indexOfLast];

		_sparseToDense[entityOfLast] = indexOfRemoved;
		_denseToSparse[indexOfRemoved] = entityOfLast;
		
		// _sparseToDense[entity] = MAX_ENTITIES;
		// _denseToSparse[entityOfLast] = MAX_ENTITIES;
		
		_sparseToDense.erase(entity);
		_denseToSparse.erase(entityOfLast);

		_size--;
	}
	
	T& GetData(Entity entity) {
		ASSERT(_sparseToDense.contains(entity), std::format("Can't get component {}. There is no such component on this {}.", typeid(T).name(), entity));
		return _components[_sparseToDense[entity]];
	}
	
	void EntityDestroyed(Entity entity) override {
		if(_sparseToDense.contains(entity) == false) {
			return;
		}
		RemoveComponent(entity);
	}

private:
	std::unordered_map<Entity, Entity> _sparseToDense{ MAX_ENTITIES };
	std::unordered_map<Entity, Entity> _denseToSparse{ MAX_ENTITIES };
	std::vector<T> _components;
	Entity _size = 0;
};
