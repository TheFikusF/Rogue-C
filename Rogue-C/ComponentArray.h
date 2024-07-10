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
	void AddComponent(Entity entity, T component);
	void RemoveComponent(Entity entity);
	T& GetData(Entity entity);
	void EntityDestroyed(Entity entity) override;

private:
	std::array<T, MAX_ENTITIES> _components;
	std::unordered_map<Entity, size_t> _entityToIndexMap;
	std::unordered_map<size_t, Entity> _indexToEntityMap;
	size_t _size;
};
