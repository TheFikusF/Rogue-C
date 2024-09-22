#pragma once
#include "./include/core/ecs/ECS.h"
#include "./include/core/networking/Client.h"
#include "./include/core/networking/Server.h"
#include <unordered_map>
#include <functional>
#include <cstdint>

namespace Core::Networking {
    struct NetworkEntity {
        std::uint8_t owner;
        Entity networkId;
        Entity localId;
        std::unordered_map<std::uint16_t, std::string> data;
    };

    class NetworkManager {
    private:
        float tick = 0.02f;
        bool isServer; 
        std::uint8_t id;

        std::unordered_map<Entity, Entity> _sceneToNetwork;
        std::unordered_map<Entity, Entity> _networkToScene;
        std::unordered_map<Entity, Signature> _localSignatures;

        std::unique_ptr<NetworkInstance> networkInstance;

        std::vector<NetworkEntity> entities;

        std::function<void (Entity)> setUpPlayer;

        NetworkManager();
        ~NetworkManager();

    public:
        static void StartServer(void (*setUpPlayer) (Entity), std::uint16_t port);
        static void StartClient(void (*setUpPlayer) (Entity), std::uint16_t port);
        static void OnConnect(Entity entity, bool isLocal);

        static void Sync();
        static void Step();
    
        static void Stop();
    };
}
