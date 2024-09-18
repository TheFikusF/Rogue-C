#pragma once
#include <cstdint>

namespace Core::Networking {
    class NetworkInstance {
    public:
        virtual void Start(std::uint16_t port) = 0;
        virtual void Step(float tick) = 0;
    };
}
