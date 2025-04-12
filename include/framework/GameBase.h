#pragma once

#include "memory/ArenaAllocator.h"
#include <raylib.h>

class GameBase
{
public:
    explicit GameBase(ArenaAllocator* arena) : m_arena(arena)
    {
        m_screenWidth = GetScreenWidth();
        m_screenHeight = GetScreenHeight();
    }

    virtual void UpdateGame(float deltaTime) = 0;
    virtual void DrawGame() = 0;
    bool ShouldClose() const { return m_shouldClose; }

protected:
    ArenaAllocator* m_arena = nullptr;

    float m_screenWidth = 0;
    float m_screenHeight = 0;

    bool m_shouldClose = false;
};
