#include "Asteroids.h"
#include "framework/GameObjectRect.h"
#include "framework/Vector2Utils.h"

namespace AsteroidsSettings
{
constexpr int PlayerWidth = 20;
constexpr int PlayerHeight = 40;
constexpr int PlayerSpeed = 180;
}  // namespace

void Asteroids::InitGame()
{
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    player = new GameObjectRect(BLACK, AsteroidsSettings::PlayerWidth, AsteroidsSettings::PlayerHeight);
    player->SetPosition({screenWidth / 2.f, screenHeight / 2.f});
}

void Asteroids::UpdateGame(float deltaTime)
{
}

void Asteroids::DrawGame()
{
    BeginDrawing();

    // Draw playground
    ClearBackground(RAYWHITE);

    // Draw player
    player->Draw();

    EndDrawing();
}

void Asteroids::UnloadGame()
{
    delete player;
}