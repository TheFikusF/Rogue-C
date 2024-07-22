#include "Entity.h"

EntityManager::EntityManager() : _entityCount(0) {
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
        _availableEntities.push(entity);
        _childToParent[entity] = MAX_ENTITIES + 1;
        _parentToChildren[entity] = std::vector<Entity>();
    }
}

Entity EntityManager::New() {
    std::unique_lock<std::mutex> lock(entityMutex);

    Entity id = _availableEntities.front();
    _availableEntities.pop();
    ++_entityCount;

    _childToParent[id] = MAX_ENTITIES + 1;
    _parentToChildren[id].clear();
 
    return id;
}

void EntityManager::Destroy(const Entity&  entity) {
    _signatures[entity].reset();
    _availableEntities.push(entity);
    --_entityCount;

    _childToParent[entity] = MAX_ENTITIES + 1;
    _parentToChildren[entity].clear();
}

void EntityManager::SetSignature(const Entity&  entity, Signature signature) {
    _signatures[entity] = signature;
}

Entity EntityManager::GetParent(const Entity&  entity) const {
    return _childToParent[entity];
}

const std::vector<Entity>& EntityManager::GetChildren(const Entity&  entity) const {
    return _parentToChildren[entity];
}

void EntityManager::SetParent(const Entity& child, const Entity& parent) {
    if(_childToParent[child] != MAX_ENTITIES + 1) {
        for (auto it = _parentToChildren[_childToParent[child]].begin(); it != _parentToChildren[_childToParent[child]].end(); ++it) {
            if (*it == child) {
                _parentToChildren[_childToParent[child]].erase(it);
                break;
            }
        }
    }

    _childToParent[child] = parent;
    _parentToChildren[parent].push_back(child);
}

Signature EntityManager::GetSignature(const Entity&  entity) const {
	return _signatures[entity];
}
