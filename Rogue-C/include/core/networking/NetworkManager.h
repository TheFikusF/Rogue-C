#pragma once
#include "./include/core/ecs/ECS.h"
#include "./include/core/networking/Client.h"
#include "./include/core/networking/Server.h"
#include <unordered_map>

namespace Core::Networking {
    class NetworkManager {
    private:
        float tick;
        bool isServer; 
        std::unordered_map<Entity, Entity> _sceneToNetwork;
        std::unordered_map<Entity, Entity> _networkToScene;
        std::unordered_map<Entity, bool> _ownershipMap;
        std::unordered_map<Entity, Signature> _localSignatures;

        std::shared_ptr<NetworkInstance> networkInstance;

    public:
        NetworkManager(/* args */);
        ~NetworkManager();

        void StartServer(std::uint16_t port);
        void StartClient(std::uint16_t port);
        void OnConnect(Entity entity, bool isLocal);

        void Sync();
        void Step();
    };
}
