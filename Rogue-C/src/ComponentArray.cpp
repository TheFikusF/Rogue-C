#include "ComponentArray.h"

//template<typename T>
//void ComponentArray<T>::AddComponent(Entity entity, T component) {
//	size_t newIndex = _size;
//	_entityToIndexMap[entity] = newIndex;
//	_indexToEntityMap[newIndex] = entity;
//	_components[newIndex] = component;
//}

//template<typename T>
//void ComponentArray<T>::RemoveComponent(Entity entity) {
//	size_t indexOfRemovedEntity = _entityToIndexMap[entity];
//	size_t indexOfLastElement = _size - 1;
//	_components[indexOfRemovedEntity] = _components[indexOfLastElement];
//
//	Entity entityOfLastElement = _indexToEntityMap[indexOfLastElement];
//	_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
//	_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;
//
//	_entityToIndexMap.erase(entity);
//	_indexToEntityMap.erase(indexOfLastElement);
//
//	--_size;
//}

//template<typename T>
//T& ComponentArray<T>::GetData(Entity entity) {
//	if (_entityToIndexMap.find(entity) == _entityToIndexMap.end()) {
//		return nullptr;
//	}
//
//	return _components[_entityToIndexMap[entity]];
//}

//template<typename T>
//void ComponentArray<T>::EntityDestroyed(Entity entity)
//{
//	if (_entityToIndexMap.find(entity) != _entityToIndexMap.end())
//	{
//		RemoveComponent(entity);
//	}
//}
