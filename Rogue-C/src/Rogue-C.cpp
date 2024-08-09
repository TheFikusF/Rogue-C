#include "Scene.h"
#include "Rogue-C.h"
#include <format>
#include <vector>
#include "Vec2.h"
#include "Player.h"
#include "Timer.h"
#include "Bullet.h"
#include "Enemy.h"
#include "ECS.h"
#include "Drawer.h"
#include "Physics.h"
#include "LOG.h"
#include "SpinningSphere.h"
#include "ParticleSystem.h"
#include "AudioManager.h"
#include "Animation.h"
#include "PickUp.h"
#include "Game.h"
#include "InputSystem.h"
#include "Button.h"
#include <assert.h>

std::vector<Scene> ConstructScenes() {
    Sprite playerSprite = SpriteManager::RegisterTexture("textures/photo_2024-07-17_14-13-38.png");
    Sprite enemySprite1 = SpriteManager::RegisterTexture("textures/photo_2024-07-17_10-53-09.png");
    Sprite enemySprite2 = SpriteManager::RegisterTexture("textures/Pasted image.png");
    Sprite enemySprite3 = SpriteManager::RegisterTexture("textures/Pasted image 1.png");
    SoundClip gospoda = AudioManager::RegisterSound("sounds/gospoda.ogg");
    Animation* animation = new Animation(playerSprite, Vec2(32, 32), Vec2(0, 0), 5);

    std::vector<Scene> scenes = {
        Scene([]() -> void {
            ECS::RegisterComponent<Player>();
            ECS::RegisterComponent<Enemy>();
            ECS::RegisterComponent<Bullet>();
            ECS::RegisterComponent<SpinningSphere>();
            ECS::RegisterComponent<PickUp>();
            ECS::RegisterComponent<Collider2D>();

        }, [animation]() -> void {
            auto physicsSystem = ECS::RegisterSystem<PhysicsSystem>();
            auto playerSystem = ECS::RegisterSystem<PlayerSystem>();
            auto enemySystem = ECS::RegisterSystem<EnemySystem>();
            auto bulletSystem = ECS::RegisterSystem<BulletSystem>();
            auto spheresSystem = ECS::RegisterSystem<SpinningSphereSystem>();
            auto pickupSystem = ECS::RegisterSystem<PickUpSystem>();

            ECS::FinishRegistering();

            Entity player = ECS::CreateEntity();
            ECS::AddComponent<Player>(player, Player{ 
                .speed = 100, 
                .health = Health(10, 0.2f, []() -> void { LOG_WARNING("PLAYER DIED"); }), 
                .shootCooldown = Timer(0.5f), 
                .abilityDuration = Timer(1.0f), 
                .abilityAmplitude = Timer(0.2f) });

            InputSystem::SetPlayer(player);
                
            ECS::AddComponent<MTransform>(player, MTransform(Vec2(GetRenderWidth() / 2, GetRenderHeight() / 2), Vec2(10, 10)));
            ECS::AddComponent<Drawer>(player, Drawer(0));
            ECS::AddComponent<Collider2D>(player, Collider2D(false, false, 5));
            ECS::AddComponent<AnimationPlayer>(player, AnimationPlayer(animation));
            enemySystem->SetUp(player, 2, 3, 4);

            Entity button = ECS::CreateEntity();
            ECS::AddComponent<MTransform>(button, MTransform(Vec2(100, 100), Vec2(100, 100)));
            ECS::AddComponent<UIDrawer>(button, UIDrawer(WHITE));
            ECS::AddComponent<Button>(button, Button());
        }),

        Scene([]() -> void {}, []() -> void {
            ECS::FinishRegistering();
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
