#pragma once

#include "framework/GameBase.h"
#include <raylib.h>

class GameObjectRect;

class Asteroids : public GameBase
{
public:
    void InitGame() override;
    void UpdateGame(float deltaTime) override;
    void DrawGame() override;
    void UnloadGame() override;

private:
    GameObjectRect* m_player = nullptr;

    float m_screenWidth = 0;
    float m_screenHeight = 0;

    Vector2 m_lastMovementDirection = {0, 0};
    float m_currentSpeed = 0;
    bool m_isMoving = false;

    Vector2 GetMovementDirection() const;
};
