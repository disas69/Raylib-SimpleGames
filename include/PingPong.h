#ifndef PINGPONG_H
#define PINGPONG_H

#include "GameBase.h"
#include <raylib.h>

class PingPong : public GameBase
{
public:
    void InitGame() override;
    void UpdateGame() override;
    void DrawGame() override;
    void UnloadGame() override;

private:
    Vector2 player1Position = {0.f, 0.f};
    Vector2 player2Position = {0.f, 0.f};
    Vector2 ballPosition = {0.f, 0.f};
    Vector2 ballDirection = {0.f, 0.f};

    int playerWidth = 20;
    int playerHeight = 100;
    int playerSpeed = 180;
    int ballRadius = 20;
    int ballSpeed = 300;

    int player1Score = 0;
    int player2Score = 0;

    int startDelay = 1;
    float startTimer = 0;

    void SpawnBall(const float screenWidth, const float screenHeight);
    void UpdatePlayer1();
    void UpdatePlayer2();
    void UpdateBall();
};

#endif  // PINGPONG_H
