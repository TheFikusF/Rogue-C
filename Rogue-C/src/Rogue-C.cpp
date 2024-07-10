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

const int WIDTH = 800;
const int HEIGHT = 450;

int main() {
    InitWindow(WIDTH, HEIGHT, "raylib [core] example - basic window");
    Player player(Vec2(400, 200), 50);
    Bullet::bullets.reserve(100);
    float spawnTime = 0;
    SetRandomSeed(GetTime());

    while (!WindowShouldClose()) {
        float ds = GetFrameTime();
        Input::Process(player.position, ds);
        Timer::ProcessAll(ds);
        Enemy::ProcessAll(ds);
        player.Process(ds);
        spawnTime += ds;

        if(spawnTime > 3) {
            spawnTime = 0;
            long r = GetRandomValue(0, 360);
            SpawnEnemy(Vec2(WIDTH / 2, HEIGHT / 2) + Vec2(sin(r), cos(r)) * WIDTH/2, &player);
        }
        
        for(int i = Bullet::bullets.size() - 1; i >= 0; i--) {
            Bullet::bullets[i].Process(ds);
            if(Bullet::bullets[i].timer > 3) {
                Bullet::bullets[i].Destroy();
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawCircle(player.position.x, player.position.y, 10, RAYWHITE);
        Enemy::DrawAll();
        for(const Bullet &bullet : Bullet::bullets) {
            DrawCircle(bullet.position.x, bullet.position.y, 5, YELLOW);
        }

        DrawText(std::format("FPS: {}", GetFPS()).c_str(), 0, 0, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}
