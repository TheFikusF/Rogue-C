#include "System.h"

namespace Core {
    namespace Debug {
        std::uint32_t totalUpdateTime = 0;
        std::uint32_t totalPhysicsTime = 0;
        std::uint32_t totalDrawTime = 0;
        std::uint32_t totalSyncTime = 0;

        std::unordered_map<const char*, std::uint32_t> updateTimings;
        std::unordered_map<const char*, std::uint32_t> physUpdateTimings;
        std::unordered_map<const char*, std::uint32_t> drawTimings;
        std::unordered_map<const char*, std::uint32_t> syncTimings;
    }

    using namespace Debug;

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
        auto totalUpdateTimeTemp = 0;
        for (auto const& pair : _systems) {
            auto updateClock = std::chrono::high_resolution_clock::now();
            pair.second->Update(dt);
            updateTimings[pair.first] = (std::chrono::high_resolution_clock::now() - updateClock).count();
            totalUpdateTimeTemp += (std::chrono::high_resolution_clock::now() - updateClock).count();
        }
        totalUpdateTime = totalUpdateTimeTemp;
    }

    void SystemManager::PhysicsUpdate(float dt) {
        auto totalUpdateTimeTemp = 0;
        for (auto const& pair : _systems) {
            auto updateClock = std::chrono::high_resolution_clock::now();
            pair.second->PhysicsUpdate(dt);
            physUpdateTimings[pair.first] = (std::chrono::high_resolution_clock::now() - updateClock).count();
            totalUpdateTimeTemp += (std::chrono::high_resolution_clock::now() - updateClock).count();
        }
        totalPhysicsTime = totalUpdateTimeTemp;
    }

    void SystemManager::Draw() {
        auto totalUpdateTimeTemp = 0;
        for (auto const& pair : _systems) {
            auto updateClock = std::chrono::high_resolution_clock::now();
            pair.second->Draw();
            drawTimings[pair.first] = (std::chrono::high_resolution_clock::now() - updateClock).count();
            totalUpdateTimeTemp += (std::chrono::high_resolution_clock::now() - updateClock).count();
        }
        totalDrawTime = totalUpdateTimeTemp;
    }

    void SystemManager::Sync() {
        auto totalUpdateTimeTemp = 0;
        for (auto const& pair : _systems) {
            auto updateClock = std::chrono::high_resolution_clock::now();
            pair.second->Sync();
            syncTimings[pair.first] = (std::chrono::high_resolution_clock::now() - updateClock).count();
            totalUpdateTimeTemp += (std::chrono::high_resolution_clock::now() - updateClock).count();
        }
        totalSyncTime = totalUpdateTimeTemp;
    }
}

