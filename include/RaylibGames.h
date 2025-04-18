#pragma once

#include "PingPong.h"
#include "Asteroids.h"
#include "Snake.h"
#include <vector>

namespace RaylibGames
{
inline const std::vector<const char*> GAME_NAMES = {"Asteroids", "Pong", "Snake"};

inline GameBase* CreateGame(const char* gameName, ArenaAllocator& arena)
{
    if (gameName == GAME_NAMES[0])
    {
        return arena.Allocate<Asteroids>(arena);
    }
    if (gameName == GAME_NAMES[1])
    {
        return arena.Allocate<PingPong>(arena);
    }
    if (gameName == GAME_NAMES[2])
    {
        return arena.Allocate<Snake>(arena);
    }

    return nullptr;
}
}  // namespace RaylibGames