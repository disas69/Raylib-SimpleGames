#pragma once

#include "PingPong.h"
#include "Asteroids.h"
#include <vector>

namespace RaylibGames
{
inline const std::vector<const char*> GAME_NAMES = {"Asteroids", "Pong"};

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

    return nullptr;
}
}  // namespace RaylibGames