#pragma once
#include "./include/core/ecs/ECS.h"
#include <unordered_map>

class NetworkManager : public Core::System
{
private:
    bool isServer; 
    std::unordered_map<Entity, Entity> _sceneToNetwork;
    std::unordered_map<Entity, Entity> _networkToScene;
    std::unordered_map<Entity, bool> _localMap;
    std::unordered_map<Entity, Signature> _localSignatures;

public:
    NetworkManager(/* args */);
    ~NetworkManager();
};