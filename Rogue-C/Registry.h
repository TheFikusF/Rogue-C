#pragma once
#include <unordered_map>
#include "Entity.h"
#include "Player.h"

struct Registry {
	std::unordered_map<Entity, Transform> transforms;
	std::unordered_map<Entity, Player> players;
	std::unordered_map<Entity, Enemy> enemies;
	std::unordered_map<Entity, Bullet> bullets;
};