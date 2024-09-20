#include "./include/core/networking/NetworkManager.h"

using namespace Core::Networking;

static NetworkManager* _instance;

NetworkManager::NetworkManager() { }

NetworkManager::~NetworkManager() { }

void NetworkManager::StartServer(std::uint16_t port) {
    _instance = new NetworkManager();

    _instance->networkInstance = std::make_unique<Server>();
    _instance->networkInstance->Start(port);
}

void NetworkManager::StartClient(std::uint16_t port) {
    _instance = new NetworkManager();
  
    _instance->networkInstance = std::make_unique<Client>();
    _instance->networkInstance->Start(port);
}

void NetworkManager::Step() {
    if (_instance == nullptr) {
        return;
    }

    _instance->networkInstance->Step(_instance->tick);
}

void NetworkManager::Sync() {
    if (_instance == nullptr) {
        return;
    }
}

void NetworkManager::Stop() {
    if (_instance == nullptr) {
        return;
    }
   
    _instance->networkInstance->Stop();
    _instance->networkInstance.reset();

    delete _instance;
    _instance = nullptr;
}
