#include "Asteroids.h"
#include "framework/GameObjectRect.h"
#include <raymath.h>

namespace AsteroidsSettings
{
constexpr int PLAYER_WIDTH = 20;
constexpr int PLAYER_HEIGHT = 40;
constexpr int PLAYER_SPEED = 180;
constexpr int PLAYER_ACCELERATION = 60;
}  // namespace AsteroidsSettings

void Asteroids::InitGame()
{
    m_screenWidth = GetScreenWidth();
    m_screenHeight = GetScreenHeight();

    m_player = new GameObjectRect(BLACK, AsteroidsSettings::PLAYER_WIDTH, AsteroidsSettings::PLAYER_HEIGHT);
    m_player->SetPosition({m_screenWidth / 2.f, m_screenHeight / 2.f});
}

void Asteroids::UpdateGame(float deltaTime)
{
    // Rotate player towards mouse
    Vector2 mousePosition = GetMousePosition();
    Vector2 playerPosition = m_player->GetPosition();
    Vector2 direction = Vector2Normalize(Vector2Subtract(mousePosition, playerPosition));
    float angle = atan2f(direction.y, direction.x);
    m_player->SetRotation(angle * RAD2DEG - 90);

    // Update player movement and speed
    Vector2 movementDirection = GetMovementDirection();
    if (Vector2Length(movementDirection) > 0)
    {
        m_lastMovementDirection = movementDirection;
        m_isMoving = true;
    }
    else
    {
        m_isMoving = false;
    }

    if (m_isMoving)
    {
        m_currentSpeed += AsteroidsSettings::PLAYER_ACCELERATION * deltaTime;
        if (m_currentSpeed > AsteroidsSettings::PLAYER_SPEED)
        {
            m_currentSpeed = AsteroidsSettings::PLAYER_SPEED;
        }
    }
    else
    {
        m_currentSpeed -= AsteroidsSettings::PLAYER_ACCELERATION * deltaTime;
        if (m_currentSpeed < 0)
        {
            m_currentSpeed = 0;
        }
    }

    // Update player position
    Vector2 playerMovement = Vector2Scale(m_lastMovementDirection, m_currentSpeed);
    Vector2 newPosition = Vector2Add(playerPosition, Vector2Scale(playerMovement, deltaTime));

    // Wrap around screen
    if (newPosition.x > m_screenWidth)
    {
        newPosition.x = 0;
    }
    else if (newPosition.x < 0)
    {
        newPosition.x = m_screenWidth;
    }

    if (newPosition.y > m_screenHeight)
    {
        newPosition.y = 0;
    }
    else if (newPosition.y < 0)
    {
        newPosition.y = m_screenHeight;
    }

    m_player->SetPosition(newPosition);
}

void Asteroids::DrawGame()
{
    BeginDrawing();

    // Draw playground
    ClearBackground(RAYWHITE);

    // Draw player
    m_player->Draw();

    EndDrawing();
}

void Asteroids::UnloadGame()
{
    delete m_player;
}

Vector2 Asteroids::GetMovementDirection() const
{
    Vector2 direction = {0, 0};

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        direction.y = -1;
    }

    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        direction.y = 1;
    }

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        direction.x = -1;
    }

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        direction.x = 1;
    }

    return direction;
}