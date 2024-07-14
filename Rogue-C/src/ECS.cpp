#include "ECS.h"

std::mutex ECS::ecsMutex;
std::unique_ptr<EntityManager> ECS::_entityManager;
std::unique_ptr<SystemManager> ECS::_systemManager;
std::unique_ptr<ComponentManager> ECS::_componentManager;
std::vector<Entity> ECS::scheduledForDeletion;