#include "PingPong.h"
#include "framework/GameObjectRect.h"
#include "framework/GameObjectCircle.h"
#include "framework/Vector2Utils.h"
#include <raylib.h>

namespace PingPongSettings
{
constexpr int PlayerWidth = 20;
constexpr int PlayerHeight = 100;
constexpr int PlayerSpeed = 180;
constexpr int BallRadius = 20;
constexpr int BallSpeed = 300;
}  // namespace

void PingPong::InitGame()
{
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    localPlayer = new GameObjectRect(BLUE, PingPongSettings::PlayerWidth, PingPongSettings::PlayerHeight);
    localPlayer->SetPosition({50.f, screenHeight / 2 - PingPongSettings::PlayerHeight / 2});

    botPlayer = new GameObjectRect(RED, PingPongSettings::PlayerWidth, PingPongSettings::PlayerHeight);
    botPlayer->SetPosition({screenWidth - 50.f - PingPongSettings::PlayerWidth, screenHeight / 2 - PingPongSettings::PlayerHeight / 2});

    ball = new GameObjectCircle(DARKGRAY, PingPongSettings::BallRadius);
    ResetGame();
}

void PingPong::UpdateGame(const float deltaTime)
{
    const float currentTime = GetTime();
    if (currentTime - startTimer >= startDelay)
    {
        UpdateBall(deltaTime);
    }

    UpdateLocalPlayer(deltaTime);
    UpdateBotPlayer(deltaTime);
}

void PingPong::DrawGame()
{
    BeginDrawing();

    // Draw playground
    ClearBackground(RAYWHITE);
    DrawRectangle(screenWidth / 2 - 2, 0, 4, screenHeight, GRAY);
    DrawText(TextFormat("%d", localPlayerScore), screenWidth / 2 - 100, 20, 40, BLUE);
    DrawText(TextFormat("%d", botPlayerScore), screenWidth / 2 + 50, 20, 40, RED);

    // Draw players and ball
    localPlayer->Draw();
    botPlayer->Draw();
    ball->Draw();

    EndDrawing();
}

void PingPong::UnloadGame()
{
    delete localPlayer;
    delete botPlayer;
    delete ball;
}

void PingPong::ResetGame()
{
    ball->SetPosition({screenWidth / 2, screenHeight / 2});

    const Vector2 rDirection = Vector2Utils::GetRandom(-0.8f, 0.8f, -0.2f, 0.2f);
    ballDirection = Vector2Utils::Normalize(rDirection);

    startTimer = GetTime();
}

void PingPong::UpdateBall(const float deltaTime)
{
    Vector2 ballPosition = ball->GetPosition();
    float x = ballPosition.x + ballDirection.x * PingPongSettings::BallSpeed * deltaTime;
    float y = ballPosition.y + ballDirection.y * PingPongSettings::BallSpeed * deltaTime;

    float radius = ball->GetRadius();
    if (y - radius <= 0 || y + radius >= screenHeight)
    {
        ballDirection = Vector2Utils::Reflect(ballDirection, {0, 1});
    }
    else if (x - radius <= 0)
    {
        botPlayerScore++;
        ResetGame();
        return;
    }
    else if (x + radius >= screenWidth)
    {
        localPlayerScore++;
        ResetGame();
        return;
    }

    if (ballDirection.x < 0 && CheckCollisionCircleRec(ballPosition, radius, localPlayer->GetRectangle()) && ballPosition.x > localPlayer->GetPosition().x)
    {
        const Vector2 normal = {-1, 0};
        ballDirection = Vector2Utils::Reflect(ballDirection, normal);
    }
    else if (ballDirection.x > 0 && CheckCollisionCircleRec(ballPosition, radius, botPlayer->GetRectangle()) && ballPosition.x < botPlayer->GetPosition().x)
    {
        const Vector2 normal = {1, 0};
        ballDirection = Vector2Utils::Reflect(ballDirection, normal);
    }

    ball->SetPosition({x, y});
}

void PingPong::UpdateLocalPlayer(const float deltaTime) const
{
    Rectangle localPlayerRect = localPlayer->GetRectangle();
    Vector2 newPosition = {localPlayer->GetPosition().x, GetMousePosition().y - localPlayerRect.height / 2};

    // Clamp player position to screen
    if (newPosition.y < 0)
    {
        newPosition.y = 0;
    }
    else if (newPosition.y + localPlayerRect.height > screenHeight)
    {
        newPosition.y = screenHeight - localPlayerRect.height;
    }

    localPlayer->SetPosition(newPosition);
}

void PingPong::UpdateBotPlayer(const float deltaTime) const
{
    Rectangle botPlayerRect = botPlayer->GetRectangle();
    Vector2 newPosition = botPlayer->GetPosition();
    Vector2 ballPosition = ball->GetPosition();

    // Try to follow the ball
    if (ballPosition.x > screenWidth / 2)
    {
        if (ballPosition.y < botPlayer->GetPosition().y)
        {
            newPosition.y -= PingPongSettings::PlayerSpeed * deltaTime;
        }
        else if (ballPosition.y > botPlayer->GetPosition().y + botPlayerRect.height)
        {
            newPosition.y += PingPongSettings::PlayerSpeed * deltaTime;
        }
    }

    // Clamp player position to screen
    if (newPosition.y < 0)
    {
        newPosition.y = 0;
    }
    else if (newPosition.y + botPlayerRect.height > screenHeight)
    {
        newPosition.y = screenHeight - botPlayerRect.height;
    }

    botPlayer->SetPosition(newPosition);
}
