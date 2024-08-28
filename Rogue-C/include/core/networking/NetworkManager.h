#pragma once
#include "./include/core/ecs/ECS.h"
#include <unordered_map>

class NetworkManager
{
private:
    std::unordered_map<Entity, Entity> _sceneToNetwork;
    std::unordered_map<Entity, Entity> _networkToScene;
    std::unordered_map<Entity, bool> _localMap;

public:
    NetworkManager(/* args */);
    ~NetworkManager();
};