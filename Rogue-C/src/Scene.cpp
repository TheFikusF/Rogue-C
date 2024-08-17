#include "Scene.h"
#include "InputSystem.h"
#include "Drawer.h"
#include "Animation.h"
#include "ParticleSystem.h"
#include "UIDrawer.h"
#include "Button.h"
#include "Tween.h"
#include "CameraContorl.h"

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
        ECS::RegisterComponent<Drawer>();
        ECS::RegisterComponent<ParticleSystem>();
        ECS::RegisterComponent<AnimationPlayer>();
        ECS::RegisterComponent<UIDrawer>();
        ECS::RegisterComponent<Button>();

        registerComponents();
        
        auto uiSystem = ECS::RegisterSystem<UIDrawerSystem>();
        auto buttonSystem = ECS::RegisterSystem<ButtonSystem>();
        auto inputSystem = ECS::RegisterSystem<InputSystem>();
        auto drawerSystem = ECS::RegisterSystem<DrawerSystem>();
        auto particleSystem = ECS::RegisterSystem<ParticleSystemSystem>();
        auto animationSystem = ECS::RegisterSystem<AnimationPlayerSystem>();
        auto tweenSystem = ECS::RegisterSystem<TweenSystem>();
        auto camerSystem = ECS::RegisterSystem<CameraContorl::CameraSystem>();

        onStart();
    }

    void Scene::Clear() {
        CameraContorl::Stop();
        ECS::Clear();
    }
}