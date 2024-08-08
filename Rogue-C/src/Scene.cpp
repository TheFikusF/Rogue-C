#include "Scene.h"
#include "InputSystem.h"
#include "Drawer.h"
#include "Animation.h"
#include "ParticleSystem.h"

Scene::Scene(std::function<void(void)> registerComponents, std::function<void(void)> onStart) 
    : registerComponents(registerComponents), onStart(onStart), started(false) { }

Scene::~Scene() {
    if(started) {
        Clear();
    }
}

void Scene::Start() {
    started = true;
    ECS::Init();

    ECS::RegisterComponent<MTransform>();
    ECS::RegisterComponent<Drawer>();
    ECS::RegisterComponent<ParticleSystem>();
    ECS::RegisterComponent<AnimationPlayer>();

    registerComponents();
    
    auto inputSystem = ECS::RegisterSystem<InputSystem>();
    auto drawerSystem = ECS::RegisterSystem<DrawerSystem>();
    auto particleSystem = ECS::RegisterSystem<ParticleSystemSystem>();
    auto animationSystem = ECS::RegisterSystem<AnimationPlayerSystem>();

    onStart();
}

void Scene::Clear() {
    ECS::Clear();
}
