#pragma once
#include "./include/core/ecs/Entity.h"
#include <cstdint>

namespace Core::Networking {
    class NetworkInstance {
    public:
        virtual void Start(void (*setUpPlayer) (Entity), std::uint16_t port) = 0;
        virtual void Step(float tick) = 0;
        virtual void Stop() = 0;
    };
}
