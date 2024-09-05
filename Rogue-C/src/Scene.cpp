#include "./include/core/game/Scene.h"
#include "./include/core/rendering/Drawer.h"
#include "./include/core/rendering/ParticleSystem.h"
#include "./include/core/rendering/UIDrawer.h"
#include "./include/core/UI/UIText.h"
#include "./include/core/systems/Button.h"
#include "./include/core/systems/InputSystem.h"
#include "./include/core/systems/Tween.h"
#include "./include/core/systems/CameraControl.h"
#include "./include/core/systems/Animation.h"

namespace Core {
    Scene::Scene(std::function<void(void)> onStart) 
        : onStart(onStart), started(false) { }

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
        ECS::RegisterComponent<UIText>();

        onStart();
    }

    void Scene::FinishRegistration() {
        auto drawerSystem = ECS::RegisterSystem<Rendering::DrawerSystem>();
        auto buttonSystem = ECS::RegisterSystem<ButtonSystem>();
        auto inputSystem = ECS::RegisterSystem<InputSystem>();
        auto particleSystem = ECS::RegisterSystem<ParticleSystemSystem>();
        auto animationSystem = ECS::RegisterSystem<AnimationPlayerSystem>();
        auto tweenSystem = ECS::RegisterSystem<TweenSystem>();
        auto cameraSystem = ECS::RegisterSystem<CameraContorl::CameraSystem>();
        auto uiSystem = ECS::RegisterSystem<UIDrawerSystem>();
        auto uiTextSystem = ECS::RegisterSystem<UITextSystem>();

        ECS::FinishRegistering();
    }

    void Scene::ReadScene(const char* sceneFile) {
        Serialization::Deserialize<SerializedScene>(sceneFile);
    }

    void Scene::Clear() {
        CameraContorl::Stop();
        ECS::Clear();
    }

    void SerializedScene::Read(const Serialization::Node* curent) {
        LOG("Reading: {}, {}", curent->name, curent->value);
        curent->Read<Serialization::SerializedEntity>();
    }

    void SerializedScene::Write(Serialization::Node* curent) const {
        for(auto const entity : Core::Debug::currentEntities) {
            Serialization::SerializedEntity(entity).Write(curent->AddChild(std::format("entity_{}", entity)));
        }
    }
}