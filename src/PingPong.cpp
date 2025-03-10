#include "PingPong.h"
#include "Vector2Utils.h"
#include <raylib.h>
#include <string>

void PingPong::InitGame()
{
    const float screenWidth = GetScreenWidth();
    const float screenHeight = GetScreenHeight();

    player1Position = {50.f, screenHeight / 2 - playerHeight / 2};
    player2Position = {screenWidth - 50.f - playerWidth, screenHeight / 2 - playerHeight / 2};

    SpawnBall(screenWidth, screenHeight);
}

void PingPong::UpdateGame()
{
    UpdateBall();
    UpdatePlayer1();
    UpdatePlayer2();
}

void PingPong::DrawGame()
{
    BeginDrawing();

    // Draw playground
    ClearBackground(RAYWHITE);
    DrawRectangle(GetScreenWidth() / 2 - 2, 0, 4, GetScreenHeight(), GRAY);
    DrawText(std::to_string(player1Score).c_str(), GetScreenWidth() / 2 - 100, 20, 40, BLUE);
    DrawText(std::to_string(player2Score).c_str(), GetScreenWidth() / 2 + 50, 20, 40, RED);

    // Draw players and ball
    DrawRectangle(player1Position.x, player1Position.y, playerWidth, playerHeight, BLUE);
    DrawRectangle(player2Position.x, player2Position.y, playerWidth, playerHeight, RED);
    DrawCircleV(ballPosition, ballRadius, DARKGRAY);

    EndDrawing();
}

void PingPong::UnloadGame()
{
    // De-Initialization
}

void PingPong::SpawnBall(const float screenWidth, const float screenHeight)
{
    ballPosition = {screenWidth / 2, screenHeight / 2};
    const Vector2 rDirection = Vector2Utils::GetRandom(-0.8f, 0.8f, -0.2f, 0.2f);
    ballDirection = Vector2Utils::Normalize(rDirection);

    startTimer = GetTime();
}

void PingPong::UpdatePlayer1()
{
    const float screenHeight = GetScreenHeight();
    Vector2 newPosition = {player1Position.x, GetMousePosition().y - playerHeight / 2};

    // Clamp player position to screen
    if (newPosition.y < 0)
    {
        newPosition.y = 0;
    }
    else if (newPosition.y + playerHeight > screenHeight)
    {
        newPosition.y = screenHeight - playerHeight;
    }

    player1Position = newPosition;
}

void PingPong::UpdatePlayer2()
{
    const float screenHeight = GetScreenHeight();
    const float deltaTime = GetFrameTime();

    Vector2 newPosition = player2Position;

    // Try to follow the ball
    if (ballPosition.x > GetScreenWidth() / 2)
    {
        if (ballPosition.y < player2Position.y)
        {
            newPosition.y -= playerSpeed * deltaTime;
        }
        else if (ballPosition.y > player2Position.y + playerHeight)
        {
            newPosition.y += playerSpeed * deltaTime;
        }
    }

    // Clamp player position to screen
    if (newPosition.y < 0)
    {
        newPosition.y = 0;
    }
    else if (newPosition.y + playerHeight > screenHeight)
    {
        newPosition.y = screenHeight - playerHeight;
    }

    player2Position = newPosition;
}

void PingPong::UpdateBall()
{
    const float currentTime = GetTime();
    if (currentTime - startTimer < startDelay)
    {
        return;
    }

    const float screenWidth = GetScreenWidth();
    const float screenHeight = GetScreenHeight();
    const float deltaTime = GetFrameTime();

    float x = ballPosition.x + ballDirection.x * ballSpeed * deltaTime;
    float y = ballPosition.y + ballDirection.y * ballSpeed * deltaTime;

    if (y - ballRadius <= 0 || y + ballRadius >= screenHeight)
    {
        ballDirection = Vector2Utils::Reflect(ballDirection, {0, 1});
    }
    else if (x - ballRadius <= 0)
    {
        player2Score++;
        SpawnBall(screenWidth, screenHeight);
        return;
    }
    else if (x + ballRadius >= screenWidth)
    {
        player1Score++;
        SpawnBall(screenWidth, screenHeight);
        return;
    }

    if (x - ballRadius <= player1Position.x + playerWidth && y >= player1Position.y && y <= player1Position.y + playerHeight
        && ballPosition.x - ballRadius > player1Position.x + playerWidth)
    {
        ballDirection = Vector2Utils::Reflect(ballDirection, {1, 0});
    }
    else if (x + ballRadius >= player2Position.x && y >= player2Position.y && y <= player2Position.y + playerHeight
        && ballPosition.x + ballRadius < player2Position.x)
    {
        ballDirection = Vector2Utils::Reflect(ballDirection, {1, 0});
    }

    ballPosition = {x, y};
}
