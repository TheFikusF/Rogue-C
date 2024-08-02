#include "ECS.h"

ECS* ECS::_instance = nullptr;

void ECS::Init() { 
    if(_instance != nullptr) {
        return;
    }
    
    _instance = new ECS();
}

void ECS::Clear() {
    delete _instance;
    _instance = nullptr;
}

void ECS::DestroyEntity(Entity entity) {
    _instance->_scheduledForDeletion.emplace_back(entity);
}

Entity ECS::GetEntityCount() { return _instance->_entityManager->_entityCount; }

void ECS::HandleCollisions(const std::vector<Collision2D>& collision) {
    _instance->_collisions.assign(collision.begin(), collision.end());
}

Signature ECS::GetEntitySignature(const Entity entity) {
    return _instance->_entityManager->GetSignature(entity);
}

Entity ECS::GetParent(const Entity entity) {
    return _instance->_entityManager->GetParent(entity);
}

void ECS::SetParent(const Entity& child, const Entity& parent) {
    _instance->_scheduledParents[child] = parent;
}

void ECS::FreeBin() {
    for (auto const& collision : _instance->_collisions) {
        _instance->_systemManager->HandleCollision(collision, ECS::GetEntitySignature(collision.a));
        _instance->_systemManager->HandleCollision(
            Collision2D{
                .isTrigger = collision.isTrigger,
                .hasCollision = collision.hasCollision,
                .a = collision.b,
                .b = collision.a,
                .pointA = collision.pointB,
                .pointB = collision.pointA,
                .normal = collision.normal,
                .depth = collision.depth,

            },
            ECS::GetEntitySignature(collision.b));
    }

    std::lock_guard<std::mutex> guard(_instance->ecsMutex);
    for (auto const& pair : _instance->_scheduledParents) {
        _instance->_entityManager->SetParent(pair.first, pair.second);
    }
    _instance->_scheduledParents.clear();

    for (auto const& pair : _instance->_scheduledSignatures) {
        _instance->_entityManager->SetSignature(pair.first, pair.second);
        _instance->_systemManager->EntitySignatureChanged(pair.first, pair.second);
    }
    _instance->_scheduledSignatures.clear();

    for (int i = _instance->_scheduledForDeletion.size() - 1; i >= 0; i--) {
        DestroyEntityInstantly(_instance->_scheduledForDeletion[i]);
    }
    _instance->_scheduledForDeletion.clear();
}

void ECS::Update(float dt) { 
    _instance->_systemManager->Update(dt); 
}

void ECS::PhysicsUpdate(float dt) {
    _instance->_systemManager->PhysicsUpdate(dt);
}

void ECS::Draw() { 
    _instance->_systemManager->Draw(); 
}

ECS::ECS() {
    _instance = this;
    _scheduledForDeletion.reserve(MAX_ENTITIES);
    _entityManager = std::make_unique<EntityManager>();
    _componentManager = std::make_unique<ComponentManager>();
    _systemManager = std::make_unique<SystemManager>();
}

void ECS::DestroyEntityInstantly(Entity entity) {
    for (auto const& child : _instance->_entityManager->GetChildren(entity)) {
        DestroyEntityInstantly(child);
    }

    _instance->_systemManager->EntityDestroyed(entity);
    _instance->_componentManager->EntityDestroyed(entity);
    _instance->_entityManager->Destroy(entity);
}
