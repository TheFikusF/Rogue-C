#pragma once
#include "./include/core/ecs/ECS.h"
#include "./include/core/Timer.h"
#include "./include/core/Gradient.h"
#include "./include/raylib/raylib.h"
#include "./include/core/Vec2.h"
#include <vector>
#include <array>

struct Particle
{
	Entity attachedSystem;
	Timer lifetime;
	Vec2 position;
	Vec2 velocity;
	float scale;
};

struct ParticleSystem : public Serialization::Serializable {
	bool loop;
	bool destroyOnStop;
	float speed;
	Gradient gradient;
	Timer lifetime;
	Timer spawnrate;
	Vec2 scaleToTime;

	ParticleSystem();

	void Read(const Serialization::Node* current) override;
	void Write(Serialization::Node* current) const override;
};

class ParticleSystemSystem : public Core::System {
private:
	std::queue<uint32_t> particlesQueue;
	std::array<Particle, MAX_ENTITIES * 4> particles;

public:
	ParticleSystemSystem();
	
	void Update(float dt) override;
	void Draw() override;

	static void Spawn(Vec2 position, bool loop);
};