#pragma once
#include "./include/core/networking/NetworkInstance.h"
#include "./include/core/ecs/ECS.h"  
#include <unordered_map>

namespace Core::Networking {
    class Server : public NetworkInstance {
    private:
        int serverSocket;
        std::unordered_map<int, Entity> clients;

    public:
        void Start(std::uint16_t port) override;
        void Step(float tick) override;
        void Stop() override;
    };
}
