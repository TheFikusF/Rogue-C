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
#include <mutex>
#include <barrier>

const int WIDTH = 800;
const int HEIGHT = 450;

std::atomic<bool> gameRunning(true);

std::mutex physicsMutex;
std::mutex drawingMutex;

float mainDt = 1;
float physicsDt = 1;

std::barrier barrier(3, []() noexcept {
    //LOG("sync start");
    ECS::FreeBin();
    //LOG("sync done");
});

auto playerClock = std::chrono::high_resolution_clock::now();
auto bulletClock = std::chrono::high_resolution_clock::now();
auto spheresClock = std::chrono::high_resolution_clock::now();
auto enemyClock = std::chrono::high_resolution_clock::now();
auto physicsClock = std::chrono::high_resolution_clock::now();
auto drawerClock = std::chrono::high_resolution_clock::now();
auto endClock = std::chrono::high_resolution_clock::now();

uint32_t playerTime = 0;
uint32_t bulletTime = 0;
uint32_t spheresTime = 0;
uint32_t enemyTime = 0;
uint32_t physicsTime = 0;
uint32_t drawerTime = 0;
uint32_t total = 0;

static void ProcessMain(std::shared_ptr<PlayerSystem> playerSystem,
    std::shared_ptr<BulletSystem> bulletSystem, 
    std::shared_ptr<SpinningSphereSystem> spheresSystem,
    std::shared_ptr<EnemySystem> enemySystem,
    std::shared_ptr<PhysicsSystem> physicsSystem,
    std::shared_ptr<DrawerSystem> drawerSystem) {

    float previousTime = GetTime();
    while (!WindowShouldClose()) {
        float currentTime = GetTime();
        mainDt = currentTime - previousTime;
        previousTime = currentTime;
        
        //LOG("main start");
        Input::Process(ECS::GetComponent<MTransform>(0).position, mainDt);

        playerClock = std::chrono::high_resolution_clock::now();
        playerSystem->Update(mainDt);
        bulletClock = std::chrono::high_resolution_clock::now();
        bulletSystem->Update(mainDt);
        spheresClock = std::chrono::high_resolution_clock::now();
        spheresSystem->Update(mainDt);
        enemyClock = std::chrono::high_resolution_clock::now();
        enemySystem->Update(mainDt);
        endClock = std::chrono::high_resolution_clock::now();
        //LOG("main done");

        playerTime = (bulletClock - playerClock).count();
        bulletTime = (spheresClock - bulletClock).count();
        spheresTime = (enemyClock - spheresClock).count();
        enemyTime = (endClock - enemyClock).count();
        total = playerTime + bulletTime + spheresTime + enemyTime;
        barrier.arrive_and_wait();
    }

    gameRunning = false;
}

static void ProcessPhysics(std::shared_ptr<PhysicsSystem> physicsSystem) {
    float previousTime = GetTime();
    while (gameRunning) {
        float currentTime = GetTime();
        physicsDt = currentTime - previousTime;
        previousTime = currentTime;
            
        //LOG("physics start");
        physicsSystem->Update(physicsDt);
        //LOG("physics done");

        barrier.arrive_and_wait();
    }
}

int main() {
    InitWindow(WIDTH, HEIGHT, "Rogue-C");
    OPEN_LOG();
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

    std::thread mainThread(ProcessMain, playerSystem, bulletSystem, spheresSystem, enemySystem, physicsSystem, drawerSystem); 
    std::thread physicsThread(ProcessPhysics, physicsSystem); 

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        //LOG("drawing start");
        drawerSystem->Update();
        //LOG("drawing done");

#ifdef DEBUG_PANEL
        uint32_t sum = physicsSystem->findTime + physicsSystem->resolveTime + physicsSystem->correctTime;
        uint32_t totalT = std::max(std::max(sum, total), drawerSystem->drawTime);
        totalT = std::max(totalT, 1u);
        total = std::max(total, 1u);
        sum = std::max(sum, 1u);

        DrawRectangle(0, 0, 100, 130, Color(0, 0, 0, 80));
        DrawText(std::format("entityCount: {}", ECS::GetEntityCount()).c_str(), 0, 10, 10, WHITE);
        DrawText(std::format("player: {}%, {}", playerTime * 100 / total, (bulletClock - playerClock).count()).c_str(), 0, 20, 10, WHITE);
        DrawText(std::format("bullet: {}%, {}", bulletTime * 100 / total, (spheresClock - bulletClock).count()).c_str(), 0, 30, 10, WHITE);
        DrawText(std::format("spheres: {}%, {}", spheresTime * 100 / total, (enemyClock - spheresClock).count()).c_str(), 0, 40, 10, WHITE);
        DrawText(std::format("enemy: {}%, {}", enemyTime * 100 / total, (endClock - enemyClock).count()).c_str(), 0, 50, 10, WHITE);
        DrawText(std::format("total: {}%, {}", total * 100 / totalT, total).c_str(), 0, 60, 10, WHITE);
        DrawText("----------------", 0, 70, 10, WHITE);
        DrawText(std::format("drawer: {}%, {}", drawerSystem->drawTime * 100 / totalT, drawerSystem->drawTime).c_str(), 0, 80, 10, WHITE);
        DrawText("----------------", 0, 90, 10, WHITE);
        uint32_t a = physicsSystem->findTime * 100 / sum;
        uint32_t b = physicsSystem->resolveTime * 100 / sum;
        uint32_t c = physicsSystem->correctTime * 100 / sum;
        DrawText(std::format("find: {}%, {}", a, physicsSystem->findTime).c_str(), 0, 100, 10, WHITE);
        DrawText(std::format("resolve: {}%, {}", b, physicsSystem->resolveTime).c_str(), 0, 110, 10, WHITE);
        DrawText(std::format("correct: {}%, {}", c, physicsSystem->correctTime).c_str(), 0, 120, 10, WHITE);
        DrawText(std::format("total: {}%, {}", sum * 100 / totalT , sum).c_str(), 0, 130, 10, WHITE);
        DrawText("----------------", 0, 140, 10, WHITE);
#endif
        DrawText(std::format("FPS: {}", GetFPS()).c_str(), 0, 0, 10, WHITE);
        EndDrawing();
        barrier.arrive_and_wait();
    }
    
    CloseWindow(); 

    mainThread.join();
    physicsThread.join();
    CLOSE_LOG();
    return 0;
}
