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
		
	}
	
	T& GetData(Entity entity) {
		return _components[entity];
	}
	
	void EntityDestroyed(Entity entity) override {

	}

private:
	T _components[MAX_ENTITIES];
};
