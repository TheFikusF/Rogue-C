#pragma once
#include "core/networking/NetworkInstance.h"

namespace Core::Networking {
    class Client : public NetworkInstance {
    private:
        int clientSocket;

    public:
        void Start(std::uint16_t port) override;
        void Step(float tick) override;
        void Stop() override;
    };
}
