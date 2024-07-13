#include "Rogue-C.h"
#include <format>
#include <vector>
#include "Vec2.h"
#include "Input.h"
#include "Player.h"
#include "Timer.h"
#include "Bullet.h"
#include "Enemy.h"
#include "./include/raylib/raylib.h"
#include "./include/raylib/raymath.h"
#include "ECS.h"
#include "Drawer.h"
#include "Physics.h"
#include "LOG.h"
#include "SpinningSphere.h"

const int WIDTH = 800;
const int HEIGHT = 450;

int main() {
    InitWindow(WIDTH, HEIGHT, "Rogue-C");
    ECS::Init();

    ECS::RegisterComponent<MTransform>();
    ECS::RegisterComponent<Player>();
    ECS::RegisterComponent<Enemy>();
    ECS::RegisterComponent<Bullet>();
    ECS::RegisterComponent<Drawer>();
    ECS::RegisterComponent<Collider2D>();
    ECS::RegisterComponent<SpinningSphere>();

    auto playerSystem = ECS::RegisterSystem<PlayerSystem>();
    auto enemySystem = ECS::RegisterSystem<EnemySystem>();
    auto drawerSystem = ECS::RegisterSystem<DrawerSystem>();
    auto bulletSystem = ECS::RegisterSystem<BulletSystem>();
    auto spheresSystem = ECS::RegisterSystem<SpinningSphereSystem>();
    auto physicsSystem = ECS::RegisterSystem<PhysicsSystem>();

    Entity player = ECS::CreateEntity();
    ECS::AddComponent<Player>(player, Player{ .speed = 50, .canShoot = true, .shootCooldown = Timer(0.2f) });
    ECS::AddComponent<MTransform>(player, MTransform{ .position = Vec2(WIDTH/2, HEIGHT/2), .scale = Vec2(10, 10) });
    ECS::AddComponent<Drawer>(player, Drawer{ .color = WHITE });
    ECS::AddComponent<Collider2D>(player, Collider2D{ .isTrigger = false, .useGravity = false, .kinematic = true,  .mass = 5, .force = Vec2(), .velocity = Vec2() });

    SetRandomSeed(GetTime());
    enemySystem->SetPlayer(player);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Input::Process(ECS::GetComponent<MTransform>(player).position, dt);

        playerSystem->Update(dt);
        bulletSystem->Update(dt);
        spheresSystem->Update(dt);
        enemySystem->Update(dt);

        physicsSystem->Update(dt);

        BeginDrawing();
        ClearBackground(BLACK);

        drawerSystem->Update(dt);

        DrawText(std::format("FPS: {}", GetFPS()).c_str(), 0, 0, 10, LIGHTGRAY);
        DrawText(std::format("entityCount: {}", ECS::GetEntityCount()).c_str(), 0, 10, 10, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}
