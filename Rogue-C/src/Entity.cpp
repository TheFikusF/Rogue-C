#include "Entity.h"

EntityManager::EntityManager() : _entityCount(0) {
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
        _availableEntities.push(entity);
    }
}

Entity EntityManager::New() {
    Entity id = _availableEntities.front();
    _availableEntities.pop();
    ++_entityCount;
    
    return id;
}

void EntityManager::Destroy(Entity entity) {
    _signatures[entity].reset();
    _availableEntities.push(entity);
    --_entityCount;
}

void EntityManager::SetSignature(Entity entity, Signature signature) {
    _signatures[entity] = signature;
}

Signature EntityManager::GetSignature(Entity entity) {
	return _signatures[entity];
}
