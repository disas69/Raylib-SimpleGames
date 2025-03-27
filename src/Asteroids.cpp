#include "Asteroids.h"
#include "framework/GameObjectRect.h"
#include <raymath.h>

namespace AsteroidsSettings
{
constexpr int PLAYER_WIDTH = 20;
constexpr int PLAYER_HEIGHT = 40;
constexpr int PLAYER_SPEED = 180;
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
    Vector2 mousePosition = GetMousePosition();
    Vector2 playerPosition = m_player->GetPosition();
    Vector2 direction = Vector2Normalize(Vector2Subtract(mousePosition, playerPosition));

    float angle = Vector2Angle({1, 0}, direction);

    m_player->SetRotation(angle * RAD2DEG);
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