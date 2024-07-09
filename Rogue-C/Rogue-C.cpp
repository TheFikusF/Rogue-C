// Rogue-C.cpp : Defines the entry point for the application.
//

#include "Rogue-C.h"
#include <format>
#include <vector>
#include "Vec2.h"
#include "./include/raylib.h"
#include "./include/raymath.h"

using namespace std;

class Player {
public:
    Vec2 position;
    float speed = 5;

public:
    Player(Vec2 position, float speed) 
        : position(position), speed(speed) { }

    void Move(Vec2 direction, float ds) {
        position += direction.GetNormalized() * ds * speed;
    }
};

class Bullet {
public:
    Vec2 position;
    Vec2 direction;
    float speed;

public:
    Bullet(Vec2 postion, Vec2 direction, float speed)
        : position(position), direction(direction.GetNormalized()), speed(speed) { }

    void Process(float ds) {
        position += direction * speed * ds;
    }
};

int main() {
    InitWindow(800, 450, "raylib [core] example - basic window");
    Player player(Vec2(400, 200), 50);
    std::vector<Bullet> bullets;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        Vec2 direction;
        if(IsKeyDown(KEY_A)) {
            direction.x = -1;
        }

        if(IsKeyDown(KEY_D)) {
            direction.x = 1;
        }

        if(IsKeyDown(KEY_W)) {
            direction.y = -1;
        }

        if(IsKeyDown(KEY_S)) {
            direction.y = 1;
        }

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            bullets.push_back(Bullet(player.position, Vec2(GetMouseX(), GetMouseY()) - player.position, 100));
        }

        player.Move(direction, GetFrameTime());
        for(auto &bullet : bullets) {
            bullet.Process(GetFrameTime());
        }

        DrawCircle(player.position.x, player.position.y, 10, RAYWHITE);
        for(auto &bullet : bullets) {
            DrawCircle(bullet.position.x, bullet.position.y, 5, YELLOW);
        }

        DrawText(std::format("FPS: {}", GetFPS()).c_str(), 0, 0, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}
