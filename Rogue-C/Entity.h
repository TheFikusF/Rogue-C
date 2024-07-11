#pragma once
#include <queue>
#include <cstdint>
#include <bitset>
#include <array>

using Entity = std::uint32_t;
using ComponentType = std::uint8_t;

const Entity MAX_ENTITIES = 5000;
const ComponentType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;

class EntityManager {
public:
	EntityManager();

	Entity New();

	void Destroy(Entity entity);

	void SetSignature(Entity entity, Signature signature);

	Signature GetSignature(Entity entity);


private:
	std::queue<Entity> _availableEntities{};
	std::array<Signature, MAX_ENTITIES> _signatures{};
	std::uint32_t _entityCount;
};
