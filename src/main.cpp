#include "framework/GameBase.h"
#include "PingPong.h"
#include "Asteroids.h"
#include "Menu.h"
#include <vector>
#include <raylib.h>

namespace RaylibGames
{
static const std::vector<const char*> GAME_NAMES = {"Asteroids", "Pong"};

static GameBase* CreateGame(const char* gameName, ArenaAllocator& arena)
{
    if (gameName == GAME_NAMES[0])
    {
        return arena.Allocate<Asteroids>(&arena);
    }
    if (gameName == GAME_NAMES[1])
    {
        return arena.Allocate<PingPong>(&arena);
    }

    return nullptr;
}
}  // namespace RaylibGames

int main()
{
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    const char* title = "Raylib Games";
    InitWindow(screenWidth, screenHeight, title);

    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    GameBase* game = nullptr;
    ArenaAllocator arena(2048);
    Menu menu(title, RaylibGames::GAME_NAMES);

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
