#pragma once
#include <queue>
#include "ECS.h"

using Entity = std::uint32_t;

const Entity MAX_ENTITIES = 5000;

class EntityManager {
public:
	EntityManager() : _entityCount(0) {
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			_availableEntities.push(entity);
		}
	}

	Entity New() {
		Entity id = _availableEntities.pop();
		_entityCount++;

		return id;
	}

	void Destroy(Entity entity) {
		_entityCount--;
		_availableEntities.push(entity);

	}

private:
	std::queue<Entity> _availableEntities;
	int _entityCount;
};
