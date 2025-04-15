#include "framework/GameBase.h"
#include "Menu.h"
#include "RaylibGames.h"
#include <raylib.h>

int main()
{
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    const char* title = "Raylib Games";
    InitWindow(screenWidth, screenHeight, title);

    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    ArenaAllocator arena(1024 * 5);

    Menu menu(title, RaylibGames::GAME_NAMES);
    GameBase* game = nullptr;

    while (!WindowShouldClose())
    {
        if (game == nullptr)
        {
            const char* gameName = menu.DrawGameSelection();
            if (gameName != nullptr)
            {
                game = RaylibGames::CreateGame(gameName, arena);
            }
        }
        else
        {
            game->UpdateGame(GetFrameTime());
            game->DrawGame();

            if (game->ShouldClose())
            {
                game = nullptr;
                arena.Reset();
            }
        }
    }

    arena.Reset();
    CloseWindow();
    return 0;
}
