#ifndef PINGPONG_H
#define PINGPONG_H

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
    GameObjectRect* localPlayer = nullptr;
    GameObjectRect* botPlayer = nullptr;
    GameObjectCircle* ball = nullptr;

    Vector2 ballDirection = {0.f, 0.f};

    float screenWidth = 0;
    float screenHeight = 0;

    int localPlayerScore = 0;
    int botPlayerScore = 0;

    int startDelay = 1;
    float startTimer = 0;

    void ResetGame();
    void UpdateBall(float deltaTime);
    void UpdateLocalPlayer(float deltaTime) const;
    void UpdateBotPlayer(float deltaTime) const;
};

#endif  // PINGPONG_H
