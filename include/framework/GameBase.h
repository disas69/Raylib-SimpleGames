#pragma once

#include <raylib.h>

class GameBase
{
public:
    GameBase()
    {
        m_screenWidth = GetScreenWidth();
        m_screenHeight = GetScreenHeight();
    }

    virtual ~GameBase() = default;
    virtual void UpdateGame(float deltaTime) = 0;
    virtual void DrawGame() = 0;

protected:
    float m_screenWidth = 0;
    float m_screenHeight = 0;
};
