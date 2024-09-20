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

        std::unique_ptr<NetworkInstance> networkInstance;

        NetworkManager();
        ~NetworkManager();

    public:
        static void StartServer(std::uint16_t port);
        static void StartClient(std::uint16_t port);
        static void OnConnect(Entity entity, bool isLocal);

        static void Sync();
        static void Step();
    
        static void Stop();
    };
}
