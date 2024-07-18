#pragma once
#include <queue>
#include <cstdint>
#include <bitset>
#include <array>
#include <unordered_map>
#include <mutex>

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

	Entity GetParent(const Entity entity);
	
	void SetParent(const Entity& child, const Entity& parent);

	std::uint32_t _entityCount;

private:
	std::mutex entityMutex;
	
	std::queue<Entity> _availableEntities{};
	std::array<Signature, MAX_ENTITIES> _signatures{};

	std::queue<Entity> _scheduledToAdd{};
	
	std::unordered_map<Entity, Entity> _childToParent{};
	std::unordered_map<Entity, std::vector<Entity>> _parentToChildren{};
};
