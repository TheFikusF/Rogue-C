#include "Scene.h"
#include "Rogue-C.h"
#include <format>
#include <vector>
#include "Vec2.h"
#include "Input.h"
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
#include <assert.h>

const uint8_t SCENES_COUNT = 1;

Scene* ConstructScenes() {
    Sprite playerSprite = SpriteManager::RegisterTexture("textures/photo_2024-07-17_14-13-38.png");
    Sprite enemySprite1 = SpriteManager::RegisterTexture("textures/photo_2024-07-17_10-53-09.png");
    Sprite enemySprite2 = SpriteManager::RegisterTexture("textures/Pasted image.png");
    Sprite enemySprite3 = SpriteManager::RegisterTexture("textures/Pasted image 1.png");
    SoundClip gospoda = AudioManager::RegisterSound("sounds/gospoda.ogg");
    Animation* animation = new Animation(playerSprite, Vec2(32, 32), Vec2(0, 0), 5);

    Scene scenes[] = {
        Scene([animation]() -> void {
            ECS::RegisterComponent<MTransform>();
            ECS::RegisterComponent<Player>();
            ECS::RegisterComponent<Enemy>();
            ECS::RegisterComponent<Bullet>();
            ECS::RegisterComponent<Drawer>();
            ECS::RegisterComponent<Collider2D>();
            ECS::RegisterComponent<SpinningSphere>();
            ECS::RegisterComponent<ParticleSystem>();
            ECS::RegisterComponent<PickUp>();
            ECS::RegisterComponent<AnimationPlayer>();

            auto playerSystem = ECS::RegisterSystem<PlayerSystem>();
            auto enemySystem = ECS::RegisterSystem<EnemySystem>();
            auto drawerSystem = ECS::RegisterSystem<DrawerSystem>();
            auto bulletSystem = ECS::RegisterSystem<BulletSystem>();
            auto spheresSystem = ECS::RegisterSystem<SpinningSphereSystem>();
            auto physicsSystem = ECS::RegisterSystem<PhysicsSystem>();
            auto pickupSystem = ECS::RegisterSystem<PickUpSystem>();
            auto particleSystem = ECS::RegisterSystem<ParticleSystemSystem>();
            auto animationSystem = ECS::RegisterSystem<AnimationPlayerSystem>();

            Entity player = ECS::CreateEntity();
            ECS::AddComponent<Player>(player, Player{ 
                .speed = 50, 
                .health = Health(10, 0.2f, []() -> void { LOG_WARNING("PLAYER DIED"); }), 
                .shootCooldown = Timer(0.5f), 
                .abilityDuration = Timer(1.0f), 
                .abilityAmplitude = Timer(0.2f) });
                
            ECS::AddComponent<MTransform>(player, MTransform(Vec2(GetRenderWidth() / 2, GetRenderHeight() / 2), Vec2(10, 10)));
            ECS::AddComponent<Drawer>(player, Drawer(0));
            ECS::AddComponent<Collider2D>(player, Collider2D(false, false, 5));
            ECS::AddComponent<AnimationPlayer>(player, AnimationPlayer(animation));
            enemySystem->SetUp(player, 2, 3, 4);
        }),

        //Scene([animation]() -> void {}),
    };

    static_assert(sizeof(scenes) / sizeof(scenes[0]) == SCENES_COUNT);

    return scenes;
}

int main() {
    Game game;

    Scene* scenes = ConstructScenes();
    
    game.AddScenes(scenes, SCENES_COUNT);
    game.Run();

    return 0;
}
