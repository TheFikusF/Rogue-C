#include "./include/core/rendering/ParticleSystem.h"
#include "./include/core/rendering/ESRendering.h"
#include "./include/core/systems/CameraControl.h"
#include "./include/core/Transform.h"
#include "./include/raylib/raymath.h"

using namespace Core;

ParticleSystem::ParticleSystem()
	: loop(true), destroyOnStop(false), speed(0), gradient(), lifetime(0), spawnrate(0), scaleToTime() { }

template<>
void Serialization::Read<ParticleSystem>(const Serialization::Node* current, ParticleSystem& target) {
	if (current->name.compare("loop") == 0) target.loop = std::stoi(current->value);
	if (current->name.compare("destroyOnStop") == 0) target.destroyOnStop = std::stoi(current->value);
	if (current->name.compare("speed") == 0) target.speed = std::stof(current->value);
	if (current->name.compare("gradient") == 0) target.gradient = current->Read<Gradient>();
	if (current->name.compare("lifetime") == 0) target.lifetime = std::stof(current->value);
	if (current->name.compare("spawnrate") == 0) target.spawnrate = std::stof(current->value);
	if (current->name.compare("scaleToTime") == 0) target.scaleToTime = current->Read<Vec2>();
}

template<>
void Serialization::Write<ParticleSystem>(Serialization::Node* current, const ParticleSystem& from) {
	current->AddChild("loop", std::to_string(from.loop));
	current->AddChild("destroyOnStop", std::to_string(from.destroyOnStop));
	current->AddChild("speed", std::to_string(from.speed));
	Serialization::Write(current->AddChild("gradient"), from.gradient);
	current->AddChild("lifetime", std::to_string(from.lifetime.time));
	current->AddChild("spawnrate", std::to_string(from.spawnrate.time));
	Serialization::Write(current->AddChild("scaleToTime"), from.scaleToTime);
}

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
			std::uint32_t particle = particlesQueue.front();
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

	for (std::uint32_t i = 0; i < MAX_ENTITIES * 4; i++) {
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
    BeginMode2D(CameraContorl::GetCurrent());
	for (Particle const& particle : particles) {
		if (particle.lifetime.IsStarted() == false) {
			continue;
		}

		if(ECS::HasComponent<ParticleSystem>(particle.attachedSystem) == false) {
			continue;
		}

		const ParticleSystem& ps = ECS::GetComponent<ParticleSystem>(particle.attachedSystem);
		Color color = ps.gradient.Evaluate(particle.lifetime.GetProgress());
		//DrawCircle(particle.position.x, particle.position.y, particle.scale, color);
        Rendering::ESRenderer::Push(0, 1, MTransform(particle.position, Vec2(particle.scale, particle.scale), 0), color);
	}
	EndMode2D();
}

void ParticleSystemSystem::Spawn(Vec2 position, bool loop) {
	Entity ps = ECS::CreateEntity();
	ECS::AddComponent<MTransform>(ps, MTransform(position));
	
	ParticleSystem s;
	s.loop = loop;
	s.destroyOnStop = !loop;
	s.speed = 200;
	s.gradient = Gradient({ GradientKey(RED, 0), GradientKey(BLACK, 1) });
	s.lifetime = 1;
	s.spawnrate = 0.1f;
	s.scaleToTime = Vec2(5, 0);
	s.lifetime.Start();
	s.spawnrate.Start();

	ECS::AddComponent<ParticleSystem>(ps, s);
}
