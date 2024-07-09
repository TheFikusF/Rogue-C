// Rogue-C.cpp : Defines the entry point for the application.
//

#include "Rogue-C.h"
#include "./include/raylib.h"

using namespace std;

int main()
{
    InitWindow(800, 450, "raylib [core] example - basic window");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow(); 
    return 0;
}
