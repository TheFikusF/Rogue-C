#include "./include/core/game/Scene.h"
#include "./include/core/rendering/Drawer.h"
#include "./include/core/rendering/ParticleSystem.h"
#include "./include/core/rendering/UIDrawer.h"
#include "./include/core/systems/Button.h"
#include "./include/core/systems/InputSystem.h"
#include "./include/core/systems/Tween.h"
#include "./include/core/systems/CameraControl.h"
#include "./include/core/systems/Animation.h"

namespace Core {
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
        CameraContorl::Init();

        ECS::RegisterComponent<MTransform>();
        ECS::RegisterComponent<Rendering::Drawer>();
        ECS::RegisterComponent<ParticleSystem>();
        ECS::RegisterComponent<AnimationPlayer>();
        ECS::RegisterComponent<UIDrawer>();
        ECS::RegisterComponent<Button>();

        registerComponents();
        
        auto buttonSystem = ECS::RegisterSystem<ButtonSystem>();
        auto inputSystem = ECS::RegisterSystem<InputSystem>();
        auto drawerSystem = ECS::RegisterSystem<Rendering::DrawerSystem>();
        auto particleSystem = ECS::RegisterSystem<ParticleSystemSystem>();
        auto animationSystem = ECS::RegisterSystem<AnimationPlayerSystem>();
        auto tweenSystem = ECS::RegisterSystem<TweenSystem>();
        auto camerSystem = ECS::RegisterSystem<CameraContorl::CameraSystem>();
        auto uiSystem = ECS::RegisterSystem<UIDrawerSystem>();

        onStart();
    }

    void Scene::Clear() {
        CameraContorl::Stop();
        ECS::Clear();
    }
}