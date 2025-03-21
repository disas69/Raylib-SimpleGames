#pragma once

#include "framework/GameBase.h"
#include <raylib.h>

class GameObjectRect;
class GameObjectCircle;

class PingPong : public GameBase
{
public:
    void InitGame() override;
    void UpdateGame(float deltaTime) override;
    void DrawGame() override;
    void UnloadGame() override;

private:
    GameObjectRect* m_localPlayer = nullptr;
    GameObjectRect* m_botPlayer = nullptr;
    GameObjectCircle* m_ball = nullptr;

    Vector2 m_ballDirection = {0.f, 0.f};

    float m_screenWidth = 0;
    float m_screenHeight = 0;

    int m_localPlayerScore = 0;
    int m_botPlayerScore = 0;

    int m_startDelay = 1;
    float m_startTimer = 0;

    void ResetGame();
    void UpdateBall(float deltaTime);
    void UpdateLocalPlayer(float deltaTime) const;
    void UpdateBotPlayer(float deltaTime) const;
};
