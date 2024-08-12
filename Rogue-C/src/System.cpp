#include "System.h"

void SystemManager::FinishRegistering() {
    for (auto pair : _systems) {
        _signatures[pair.first] = pair.second->signature;
    }
}

void SystemManager::EntityDestroyed(Entity entity) {
    for (auto const& pair : _systems) {
        auto const& system = pair.second;

        system->OnEntityRemoved(entity);
        system->Entities.erase(entity);
    }
}

void SystemManager::EntitySignatureChanged(Entity entity, Signature entitySignature) {
    for (auto const& pair : _systems) {
        auto const& type = pair.first;
        auto const& system = pair.second;
        auto const& systemSignature = _signatures[type];

        if ((entitySignature & systemSignature) == systemSignature) {
            system->Entities.insert(entity);
            system->OnEntityAdded(entity);
        } else {
            if(system->Entities.contains(entity)) {
                system->OnEntityRemoved(entity);
                system->Entities.erase(entity);
            }
        }
    }
}

void SystemManager::HandleCollision(const Collision2D& collision, const Signature entitySignature) {
    for (auto const& pair : _systems) {
        auto const& type = pair.first;
        auto const& system = pair.second;
        auto const& systemSignature = _signatures[type];

        if ((entitySignature & systemSignature) != systemSignature) {
            continue;
        }

        if (collision.isTrigger) {
            system->OnTrigger(collision);
        } else {
            system->OnCollision(collision);
        }
    }
}

void SystemManager::Update(float dt) {
    for (auto const& pair : _systems) {
        auto updateClock = std::chrono::high_resolution_clock::now();
        pair.second->Update(dt);
        _updateTimings[pair.first] = (std::chrono::high_resolution_clock::now() - updateClock).count();
    }
}

void SystemManager::PhysicsUpdate(float dt) {
    for (auto const& pair : _systems) {
        auto updateClock = std::chrono::high_resolution_clock::now();
        pair.second->PhysicsUpdate(dt);
        _physUpdateTimings[pair.first] = (std::chrono::high_resolution_clock::now() - updateClock).count();
    }
}

void SystemManager::Draw() {
    for (auto const& pair : _systems) {
        auto updateClock = std::chrono::high_resolution_clock::now();
        pair.second->Draw();
        _drawTimings[pair.first] = (std::chrono::high_resolution_clock::now() - updateClock).count();
    }
}
