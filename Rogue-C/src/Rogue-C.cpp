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
#include <Drawer.h>

const int WIDTH = 800;
const int HEIGHT = 450;

int main() {
    InitWindow(WIDTH, HEIGHT, "raylib [core] example - basic window");
    ECS::Init();

    ECS::RegisterComponent<MTransform>();
    ECS::RegisterComponent<Player>();
    ECS::RegisterComponent<Enemy>();
    ECS::RegisterComponent<Bullet>();
    ECS::RegisterComponent<Drawer>();

    auto playerSystem = ECS::RegisterSystem<PlayerSystem>();
    auto enemySystem = ECS::RegisterSystem<EnemySystem>();
    auto drawerSystem = ECS::RegisterSystem<DrawerSystem>();
    auto bulletSystem = ECS::RegisterSystem<BulletSystem>();

    Entity player = ECS::CreateEntity();
    ECS::AddComponent<Player>(player, Player{ .speed = 50, .canShoot = true, .shootCooldown = Timer(0.2f) });
    ECS::AddComponent<MTransform>(player, MTransform{ .position = Vec2(WIDTH/2, HEIGHT/2), .scale = Vec2(10, 10) });
    ECS::AddComponent<Drawer>(player, Drawer{ .color = WHITE });
    
    float spawnTime = 0;
    SetRandomSeed(GetTime());
    enemySystem->SetPlayer(player);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Input::Process(ECS::GetComponent<MTransform>(player).position, dt);

        spawnTime += dt;

        if(spawnTime > 3) {
            spawnTime = 0;
            long r = GetRandomValue(0, 360);
            enemySystem->Spawn(Vec2(WIDTH / 2, HEIGHT / 2) + Vec2(sin(r), cos(r)) * WIDTH/2);
        }
        
        playerSystem->Update(dt);
        bulletSystem->Update(dt);
        enemySystem->Update(dt);

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
