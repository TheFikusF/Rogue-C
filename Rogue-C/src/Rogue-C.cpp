#include "Rogue-C.h"
#include <format>
#include <vector>
#include "Vec2.h"
#include "Input.h"
#include "Player.h"
#include "Timer.h"
#include "Bullet.h"
#include "./include/raylib/raylib.h"
#include "./include/raylib/raymath.h"

int main() {
    InitWindow(800, 450, "raylib [core] example - basic window");
    Player player(Vec2(400, 200), 50);
    Bullet::bullets.reserve(100);

    while (!WindowShouldClose()) {
        float ds = GetFrameTime();
        Input::Process(player.position, ds);
        Timer::ProcessAll(ds);
        player.Process(ds);
        
        for(int i = Bullet::bullets.size() - 1; i >= 0; i--) {
            Bullet::bullets[i].Process(ds);
            if(Bullet::bullets[i].timer > 3) {
                Bullet::bullets.erase(Bullet::bullets.begin() + i);
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawCircle(player.position.x, player.position.y, 10, RAYWHITE);
        for(const Bullet &bullet : Bullet::bullets) {
            DrawCircle(bullet.position.x, bullet.position.y, 5, YELLOW);
        }

        DrawText(std::format("FPS: {}", GetFPS()).c_str(), 0, 0, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}
