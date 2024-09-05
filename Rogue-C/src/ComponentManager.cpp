#include "./include/core/ecs/ComponentManager.h"

Core::ComponentManager::~ComponentManager() {
    for(auto pair : _componentArrays) {
        pair.second->Clear();
    }
}

void Core::ComponentManager::AddComponent(Entity entity, std::size_t componentTypeHash, void* data) {
    _componentArrays[componentTypeHash]->SetData(entity, data);
}

void const* Core::ComponentManager::GetComponent(Entity entity, std::size_t componentTypeHash) {
    return _componentArrays[componentTypeHash]->GetDataUntyped(entity);
}

void Core::ComponentManager::EntityDestroyed(Entity entity) {
	for (auto const& pair : _componentArrays) {
		auto const& component = pair.second;
		component->EntityDestroyed(entity);
    }
}