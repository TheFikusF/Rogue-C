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
#include <thread>
#include <atomic>

const int WIDTH = 800;
const int HEIGHT = 450;

std::atomic<bool> gameRunning(true);

void ProcessPhysics(float dt, std::shared_ptr<PhysicsSystem> physicsSystem) {
    while (gameRunning) {
        physicsSystem->Update(dt);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void ProcessDrawing(std::shared_ptr<DrawerSystem> drawerSystem) {
    while (gameRunning) {
        float dt = GetFrameTime();
        BeginDrawing();
        ClearBackground(GREEN);

        drawerSystem->Update(dt);

        EndDrawing();
    }
}

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

    auto playerClock = std::chrono::high_resolution_clock::now();
    auto bulletClock = std::chrono::high_resolution_clock::now();
    auto spheresClock = std::chrono::high_resolution_clock::now();
    auto enemyClock = std::chrono::high_resolution_clock::now();
    auto physicsClock = std::chrono::high_resolution_clock::now();
    auto drawerClock = std::chrono::high_resolution_clock::now();
    auto endClock = std::chrono::high_resolution_clock::now();

    // std::thread physicsThread(ProcessPhysics, 1.0f / 60.0f, physicsSystem); 
    // std::thread drawingThread(ProcessDrawing, drawerSystem); 

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Input::Process(ECS::GetComponent<MTransform>(player).position, dt);

        playerClock = std::chrono::high_resolution_clock::now();
        playerSystem->Update(dt);
        bulletClock = std::chrono::high_resolution_clock::now();
        bulletSystem->Update(dt);
        spheresClock = std::chrono::high_resolution_clock::now();
        spheresSystem->Update(dt);
        enemyClock = std::chrono::high_resolution_clock::now();
        enemySystem->Update(dt);

        physicsClock = std::chrono::high_resolution_clock::now();
        physicsSystem->Update(dt);

        BeginDrawing();
        ClearBackground(GREEN);

        drawerClock = std::chrono::high_resolution_clock::now();
        drawerSystem->Update(dt);
        endClock = std::chrono::high_resolution_clock::now();

        DrawRectangle(0, 0, 100, 130, Color(0, 0, 0, 80));
        DrawText(std::format("FPS: {}", GetFPS()).c_str(), 0, 0, 10, WHITE);
        DrawText(std::format("entityCount: {}", ECS::GetEntityCount()).c_str(), 0, 10, 10, WHITE);
        DrawText(std::format("player: {}", (bulletClock - playerClock).count()).c_str(), 0, 20, 10, WHITE);
        DrawText(std::format("bullet: {}", (spheresClock - bulletClock).count()).c_str(), 0, 30, 10, WHITE);
        DrawText(std::format("spheres: {}", (enemyClock - spheresClock).count()).c_str(), 0, 40, 10, WHITE);
        DrawText(std::format("enemy: {}", (physicsClock - enemyClock).count()).c_str(), 0, 50, 10, WHITE);
        DrawText(std::format("physics: {}", (drawerClock - physicsClock).count()).c_str(), 0, 60, 10, WHITE);
        DrawText(std::format("drawer: {}", (endClock - drawerClock).count()).c_str(), 0, 70, 10, WHITE);
        DrawText("----------------", 0, 80, 10, WHITE);
        DrawText(std::format("find: {}", physicsSystem->findTime).c_str(), 0, 90, 10, WHITE);
        DrawText(std::format("resolve: {}", physicsSystem->resolveTime).c_str(), 0, 100, 10, WHITE);
        DrawText(std::format("correct: {}", physicsSystem->correctTime).c_str(), 0, 110, 10, WHITE);
        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}
