#pragma once
#include "Registry.h"

class ECS
{
public:
	ECS() { }

	Registry* GetRegistry();
	EntityManager* GetEM();

private:
	EntityManager _entityManager;
	Registry _registry;
};

ECS MainECS;