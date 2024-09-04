#include "core.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "SpinningSphere.h"
#include "PickUp.h"
#include <memory>

using namespace Core;

std::vector<Scene> ConstructScenes() {
    TextureID playerSprite = SpriteManager::RegisterTexture("textures/photo_2024-07-17_14-13-38.png");
    TextureID enemyTexture1 = SpriteManager::RegisterTexture("textures/photo_2024-07-17_10-53-09.png");
    TextureID enemyTexture2 = SpriteManager::RegisterTexture("textures/Pasted image.png");
    TextureID enemyTexture3 = SpriteManager::RegisterTexture("textures/Pasted image 1.png");

    ShaderID shader0 = SpriteManager::RegisterShader("textures/shaders/posterization.fs");

    SpriteID enemySprite1 = SpriteManager::RegisterSprite(enemyTexture1);
    SpriteID enemySprite2 = SpriteManager::RegisterSprite(enemyTexture2);
    SpriteID enemySprite3 = SpriteManager::RegisterSprite(enemyTexture3);

    SoundClip gospoda = AudioManager::RegisterSound("sounds/gospoda.ogg");
    Animation* animation = new Animation(playerSprite, Vec2(32, 32), Vec2(0, 0), 5);

    std::vector<Scene> scenes = {
        Scene([animation]() -> void {
            ECS::RegisterComponent<Player>();
            ECS::RegisterComponent<Enemy>();
            ECS::RegisterComponent<Bullet>();
            ECS::RegisterComponent<SpinningSphere>();
            ECS::RegisterComponent<PickUp>();
            ECS::RegisterComponent<Collider2D>();

            auto physicsSystem = ECS::RegisterSystem<Physics::PhysicsSystem>();
            auto playerSystem = ECS::RegisterSystem<PlayerSystem>();
            auto enemySystem = ECS::RegisterSystem<EnemySystem>();
            auto bulletSystem = ECS::RegisterSystem<BulletSystem>();
            auto spheresSystem = ECS::RegisterSystem<SpinningSphereSystem>();
            auto pickupSystem = ECS::RegisterSystem<PickUpSystem>();

            Scene::FinishRegistration();

            Entity player = ECS::CreateEntity();
            ECS::AddComponent<Player>(player, Player{ 
                .speed = 100, 
                .health = Health(10, 0.2f, []() -> void { LOG_WARNING("PLAYER DIED"); }), 
                .shootCooldown = Timer(0.1f), 
                .abilityDuration = Timer(1.0f), 
                .abilityAmplitude = Timer(0.2f) });

            ECS::AddComponent<MTransform>(player, MTransform(Vec2(GetRenderWidth() / 2, GetRenderHeight() / 2), Vec2(10, 10)));
            ECS::AddComponent<Rendering::Drawer>(player, Rendering::Drawer(0));
            ECS::AddComponent<Collider2D>(player, Collider2D(false, false, 5));
            ECS::AddComponent<AnimationPlayer>(player, AnimationPlayer(animation));

            Entity button = ECS::CreateEntity();
            ECS::AddComponent<MTransform>(button, MTransform(Vec2(100, 100), Vec2(100, 100)));
            ECS::AddComponent<UIDrawer>(button, UIDrawer(WHITE));
            ECS::AddComponent<Button>(button, Button(Vec2(100, 100), []() -> void { LOG("HENLOOO"); }));

            Entity button1 = ECS::CreateEntity();
            ECS::AddComponent<MTransform>(button1, MTransform(Vec2(200, 50), Vec2(100, 50)));
            ECS::AddComponent<UIDrawer>(button1, UIDrawer(WHITE));
            ECS::AddComponent<Button>(button1, Button(Vec2(100, 50), []() -> void { LOG("HENLOOO AS WELL!!"); }));

            Entity text = ECS::CreateEntity();
            ECS::SetParent(text, button1);
            ECS::AddComponent<MTransform>(text, MTransform(Vec2(0, 0), Vec2(100, 50)));
            ECS::AddComponent<UIText>(text, UIText("rendering some text)))))))))", BLACK));
            
            enemySystem->SetUp(player, 2, 3, 4);
            InputSystem::SetPlayer(player);
            CameraContorl::SetTarget(player);
            CameraContorl::SetSmooth(0.5f);
        }),

        Scene([]() -> void {
            Scene::FinishRegistration();

            auto updateClock = std::chrono::high_resolution_clock::now();
            Scene::ReadScene("scene.txt");
            auto time_stamp = duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - updateClock);
            LOG("TIME {}", (time_stamp).count());
        }),
    };

    return scenes;
}

int main() {
    Game game;

    std::vector<Scene> scenes = ConstructScenes();
    
    game.AddScenes(scenes);
    game.Run();

    return 0;
}
