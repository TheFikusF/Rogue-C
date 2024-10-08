#include "./include/core/ecs/ComponentManager.h"

Core::ComponentManager::~ComponentManager() {
    for(auto pair : _componentArrays) {
        pair.second->Clear();
    }
}