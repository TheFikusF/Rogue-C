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
const ComponentType MAX_COMPONENTS = 128;

using Signature = std::bitset<MAX_COMPONENTS>;

class EntityManager {
public:
	std::uint32_t _entityCount;

public:
	EntityManager();

	Entity New();

	void Destroy(const Entity  entity);

	void SetSignature(const Entity  entity, Signature signature);

	Signature GetSignature(const Entity  entity) const;

	Entity GetParent(const Entity  entity) const;

	const std::vector<Entity>& GetChildren(const Entity  entity) const;
	
	void SetParent(const Entity child, const Entity parent);

private:
	std::mutex _entityMutex;
	
	std::queue<Entity> _availableEntities{};
	std::array<Signature, MAX_ENTITIES> _signatures{};

	std::queue<Entity> _scheduledToAdd{};
	
	std::array<Entity, MAX_ENTITIES> _childToParent{};
	std::array<std::vector<Entity>, MAX_ENTITIES> _parentToChildren{};
};
