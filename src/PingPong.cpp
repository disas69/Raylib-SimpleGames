#include "PingPong.h"
#include "framework/GameObjectRect.h"
#include "framework/GameObjectCircle.h"
#include "framework/Vector2Utils.h"
#include <raylib.h>

namespace PingPongSettings
{
constexpr int PLAYER_WIDTH = 20;
constexpr int PLAYER_HEIGHT = 100;
constexpr int PLAYER_SPEED = 180;
constexpr int BALL_RADIUS = 20;
constexpr int BALL_SPEED = 300;
}  // namespace PingPongSettings

void PingPong::InitGame()
{
    m_screenWidth = GetScreenWidth();
    m_screenHeight = GetScreenHeight();

    m_localPlayer = new GameObjectRect(BLUE, PingPongSettings::PLAYER_WIDTH, PingPongSettings::PLAYER_HEIGHT);
    m_localPlayer->SetPosition({50.f, m_screenHeight / 2 - PingPongSettings::PLAYER_HEIGHT / 2});

    m_botPlayer = new GameObjectRect(RED, PingPongSettings::PLAYER_WIDTH, PingPongSettings::PLAYER_HEIGHT);
    m_botPlayer->SetPosition({m_screenWidth - 50.f - PingPongSettings::PLAYER_WIDTH, m_screenHeight / 2 - PingPongSettings::PLAYER_HEIGHT / 2});

    m_ball = new GameObjectCircle(DARKGRAY, PingPongSettings::BALL_RADIUS);
    ResetGame();
}

void PingPong::UpdateGame(const float deltaTime)
{
    const float currentTime = GetTime();
    if (currentTime - m_startTimer >= m_startDelay)
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
    DrawRectangle(m_screenWidth / 2 - 2, 0, 4, m_screenHeight, GRAY);
    DrawText(TextFormat("%d", m_localPlayerScore), m_screenWidth / 2 - 100, 20, 40, BLUE);
    DrawText(TextFormat("%d", m_botPlayerScore), m_screenWidth / 2 + 50, 20, 40, RED);

    // Draw players and ball
    m_localPlayer->Draw();
    m_botPlayer->Draw();
    m_ball->Draw();

    EndDrawing();
}

void PingPong::UnloadGame()
{
    delete m_localPlayer;
    delete m_botPlayer;
    delete m_ball;
}

void PingPong::ResetGame()
{
    m_ball->SetPosition({m_screenWidth / 2, m_screenHeight / 2});

    int random = GetRandomValue(0, 1);
    if (random == 0)
    {
        m_ballDirection = Vector2Utils::Normalize({1, 0});
    }
    else
    {
        m_ballDirection = Vector2Utils::Normalize({-1, 0});
    }

    m_startTimer = GetTime();
}

void PingPong::UpdateBall(const float deltaTime)
{
    Vector2 ballPosition = m_ball->GetPosition();
    float x = ballPosition.x + m_ballDirection.x * PingPongSettings::BALL_SPEED * deltaTime;
    float y = ballPosition.y + m_ballDirection.y * PingPongSettings::BALL_SPEED * deltaTime;

    float radius = m_ball->GetRadius();
    if (y - radius <= 0 || y + radius >= m_screenHeight)
    {
        m_ballDirection = Vector2Utils::Reflect(m_ballDirection, {0, 1});
    }
    else if (x - radius <= 0)
    {
        m_botPlayerScore++;
        ResetGame();
        return;
    }
    else if (x + radius >= m_screenWidth)
    {
        m_localPlayerScore++;
        ResetGame();
        return;
    }

    if (m_ballDirection.x < 0 && CheckCollisionCircleRec(ballPosition, radius, m_localPlayer->GetRectangle()) && ballPosition.x > m_localPlayer->GetPosition().x)
    {
        float halfSize = m_localPlayer->GetRectangle().height / 2;
        float playerCenter = m_localPlayer->GetPosition().y + halfSize;
        float relativeY = (ballPosition.y - playerCenter) / halfSize;
        const Vector2 normal = Vector2Utils::Normalize({1, 0.2f * relativeY});
        m_ballDirection = Vector2Utils::Reflect(m_ballDirection, normal);
    }
    else if (m_ballDirection.x > 0 && CheckCollisionCircleRec(ballPosition, radius, m_botPlayer->GetRectangle()) && ballPosition.x < m_botPlayer->GetPosition().x)
    {
        float halfSize = m_botPlayer->GetRectangle().height / 2;
        float playerCenter = m_botPlayer->GetPosition().y + halfSize;
        float relativeY = (ballPosition.y - playerCenter) / halfSize;
        const Vector2 normal = Vector2Utils::Normalize({-1, 0.2f * relativeY});
        m_ballDirection = Vector2Utils::Reflect(m_ballDirection, normal);
    }

    m_ball->SetPosition({x, y});
}

void PingPong::UpdateLocalPlayer(const float deltaTime) const
{
    Rectangle localPlayerRect = m_localPlayer->GetRectangle();
    Vector2 newPosition = {m_localPlayer->GetPosition().x, GetMousePosition().y - localPlayerRect.height / 2};

    // Clamp player position to screen
    if (newPosition.y < 0)
    {
        newPosition.y = 0;
    }
    else if (newPosition.y + localPlayerRect.height > m_screenHeight)
    {
        newPosition.y = m_screenHeight - localPlayerRect.height;
    }

    m_localPlayer->SetPosition(newPosition);
}

void PingPong::UpdateBotPlayer(const float deltaTime) const
{
    Rectangle botPlayerRect = m_botPlayer->GetRectangle();
    Vector2 newPosition = m_botPlayer->GetPosition();
    Vector2 ballPosition = m_ball->GetPosition();

    // Try to follow the ball
    if (ballPosition.x > m_screenWidth / 2 && m_ballDirection.x > 0)
    {
        if (ballPosition.y < m_botPlayer->GetPosition().y)
        {
            newPosition.y -= PingPongSettings::PLAYER_SPEED * deltaTime;
        }
        else if (ballPosition.y > m_botPlayer->GetPosition().y + botPlayerRect.height)
        {
            newPosition.y += PingPongSettings::PLAYER_SPEED * deltaTime;
        }
    }

    // Clamp player position to screen
    if (newPosition.y < 0)
    {
        newPosition.y = 0;
    }
    else if (newPosition.y + botPlayerRect.height > m_screenHeight)
    {
        newPosition.y = m_screenHeight - botPlayerRect.height;
    }

    m_botPlayer->SetPosition(newPosition);
}
