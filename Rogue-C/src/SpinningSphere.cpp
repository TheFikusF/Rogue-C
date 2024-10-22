#include "SpinningSphere.h"

using namespace Core;

SpinningSphereSystem::SpinningSphereSystem() {
	signature.set(ECS::GetComponentType<MTransform>());
	signature.set(ECS::GetComponentType<SpinningSphere>());
	signature.set(ECS::GetComponentType<Collider2D>());
}

void SpinningSphereSystem::Update(float dt) {
    for (auto const& entity : Entities) {
        MTransform& tr = ECS::GetComponent<MTransform>(entity);
        SpinningSphere& bullet = ECS::GetComponent<SpinningSphere>(entity);
        Collider2D& collider = ECS::GetComponent<Collider2D>(entity);
        
        Vec2 target = Vec2(cos(bullet.timer), sin(bullet.timer)) * bullet.radius;
        collider.velocity = (target - tr.position).GetNormalized() * bullet.speed * 14;
        bullet.timer += dt * bullet.speed / 2;
    }
}

void SpinningSphereSystem::OnTrigger(const Collision2D& collision) {
    if (ECS::HasComponent<Enemy>(collision.b)) {
        Enemy& enemy = ECS::GetComponent<Enemy>(collision.b);
        enemy.health.TakeDamage(1);
        ECS::DestroyEntity(collision.a);
    }
}

void SpawnSphere(const Entity& parent, float size, float speed, float radius) {
    Entity entity = ECS::CreateEntity();
    ECS::AddComponent<MTransform>(entity, MTransform(Vec2(), Vec2(size, size)));
    ECS::AddComponent<SpinningSphere>(entity, SpinningSphere{ .radius = radius, .speed = speed, .timer = 0 });
    ECS::AddComponent<Rendering::Drawer>(entity, Rendering::Drawer(YELLOW));
    ECS::AddComponent<Collider2D>(entity, Collider2D(true, false, 5));
    ECS::SetParent(entity, parent);
}
