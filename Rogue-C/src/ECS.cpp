#include "ECS.h"

Registry* ECS::GetRegistry()
{
	return &_registry;
}

EntityManager* ECS::GetEM()
{
	return &_entityManager;
}
