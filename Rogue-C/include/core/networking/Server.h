#pragma once
#include "core/networking/NetworkInstance.h"

namespace Core::Networking {
    class Server : public NetworkInstance {
    public:
        void Start(std::uint16_t port) override;
        void Step(float tick) override;
    };
}
