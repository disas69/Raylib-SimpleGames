#include "framework/GameBase.h"
#include "PingPong.h"
#include "Asteroids.h"
#include <raylib.h>

int main()
{
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib Games");

    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    ArenaAllocator arena(2048);
    GameBase* game = arena.Allocate<Asteroids>(&arena);

    while (!WindowShouldClose())
    {
        game->UpdateGame(GetFrameTime());
        game->DrawGame();
    }

    arena.Reset();
    CloseWindow();
    return 0;
}
