#include "ECS.h"

std::unique_ptr<EntityManager> ECS::_entityManager;
std::unique_ptr<SystemManager> ECS::_systemManager;
std::unique_ptr<ComponentManager> ECS::_componentManager;