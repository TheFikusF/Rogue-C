#pragma once
#include <array>
#include "ECS.h"
#include "System.h"
#include <vector>
#include "./include/raylib/raylib.h"
#include "Timer.h"
#include "Transform.h"

struct Particle
{
	Entity attachedSystem;
	Timer lifetime;
	Color color;
	Vec2 position;
	Vec2 velocity;
	float scale;
};

struct ParticleSystem {
	bool loop;
	bool destroyOnStop;
	Timer lifetime;
	Timer spawnrate;
	Vec2 scaleToTime;
};

class ParticleSystemSystem : public System {
private:
	std::queue<uint32_t> particlesQueue;
	std::array<Particle, MAX_ENTITIES * 4> particles;

public:
	ParticleSystemSystem();
	
	void Update(float dt) override;
	void Draw() override;

	static void Spawn(Vec2 position, bool loop);
};