#include <GameBase.h>
#include <PingPong.h>
#include "raylib.h"

int main()
{
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib games");

    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    GameBase* game = new PingPong();
    game->InitGame();

    while (!WindowShouldClose())
    {
        game->UpdateGame();
        game->DrawGame();
    }

    game->UnloadGame();
    delete game;

    CloseWindow();
    return 0;
}
