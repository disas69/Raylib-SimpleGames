#include "framework/GameBase.h"
#include "PingPong.h"
#include "Asteroids.h"
#include <raylib.h>

int main()
{
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib games");

    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    GameBase* game = new Asteroids();
    game->InitGame();

    while (!WindowShouldClose())
    {
        game->UpdateGame(GetFrameTime());
        game->DrawGame();
    }

    game->UnloadGame();
    delete game;

    CloseWindow();
    return 0;
}
