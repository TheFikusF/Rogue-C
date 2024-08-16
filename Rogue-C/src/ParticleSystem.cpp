#include "ParticleSystem.h"
#include "./include/raylib/raymath.h"

using namespace Core;

ParticleSystemSystem::ParticleSystemSystem() {
	signature.set(ECS::GetComponentType<MTransform>());
	signature.set(ECS::GetComponentType<ParticleSystem>());
	for (uint32_t i = 0; i < MAX_ENTITIES * 4; i++) {
		particlesQueue.push(i);
	}
}

void ParticleSystemSystem::Update(float dt) {
	for (Entity const& entity : Entities) {
		const MTransform& tr = ECS::GetComponent<MTransform>(entity);
		ParticleSystem& ps = ECS::GetComponent<ParticleSystem>(entity);

		if (ps.spawnrate.Check(dt)) {
			int angle = GetRandomValue(0, 360);
			uint32_t particle = particlesQueue.front();
			particlesQueue.pop();
			particles[particle] = Particle{
				.attachedSystem = entity,
				.lifetime = Timer(1.0f),
				.position = MTransformSystem::GetRealPosition(entity),
				.velocity = Vec2(cos(angle), sin(angle)) * ps.speed,
				.scale = 5.0f
			};

			particles[particle].lifetime.Start();
			ps.spawnrate.Start();
		}

		if (ps.lifetime.Check(dt)) {
			if (ps.loop == true) {
				ps.lifetime.Start();
			}

			if (ps.destroyOnStop == true) {
				ECS::DestroyEntity(entity);
			}
		}
	}

	for (uint32_t i = 0; i < MAX_ENTITIES * 4; i++) {
		if (particles[i].lifetime.IsStarted() == false) {
			continue;
		}

		if(ECS::HasComponent<ParticleSystem>(particles[i].attachedSystem) == false) {
			particles[i].lifetime.Stop();
			continue;
		}
		
		const ParticleSystem& ps = ECS::GetComponent<ParticleSystem>(particles[i].attachedSystem);
		particles[i].scale = Lerp(ps.scaleToTime.x, ps.scaleToTime.y, particles[i].lifetime.GetProgress());
		
		particles[i].position += particles[i].velocity * dt;
		if (particles[i].lifetime.Check(dt)) {
			particlesQueue.push(i);
		}
	}
}

void ParticleSystemSystem::Draw() {
	for (Particle const& particle : particles) {
		if (particle.lifetime.IsStarted() == false) {
			continue;
		}

		if(ECS::HasComponent<ParticleSystem>(particle.attachedSystem) == false) {
			continue;
		}

		const ParticleSystem& ps = ECS::GetComponent<ParticleSystem>(particle.attachedSystem);
		Color color = ps.gradient.Evaluate(particle.lifetime.GetProgress());
		DrawCircle(particle.position.x, particle.position.y, particle.scale, color);
	}
}

void ParticleSystemSystem::Spawn(Vec2 position, bool loop) {
	Entity ps = ECS::CreateEntity();
	ECS::AddComponent<MTransform>(ps, MTransform(position));
	Timer lifetime = 1;
	Timer spawnrate = 0.1f;
	lifetime.Start();
	spawnrate.Start();
	ECS::AddComponent<ParticleSystem>(ps, ParticleSystem{ 
		.loop = loop, 
		.destroyOnStop = !loop, 
		.speed = 200,
		.gradient = Gradient({ GradientKey(RED, 0), GradientKey(BLACK, 1) }),
		.lifetime = lifetime, 
		.spawnrate = spawnrate, 
		.scaleToTime = Vec2(5, 0),
	});
}
