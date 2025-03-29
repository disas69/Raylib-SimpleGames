#pragma once

#include "framework/GameBase.h"
#include "framework/GameObjectCircle.h"
#include <raylib.h>

class GameObjectRect;

class Bullet : public GameObjectCircle
{
public:
    Bullet() = default;
    void Init(const Color& color, float radius, float screenWidth, float screenHeight);
    void Shoot(Vector2 position, Vector2 direction);
    void Update(float deltaTime);

private:
    Vector2 m_direction = {0, 0};

    float m_screenWidth = 0;
    float m_screenHeight = 0;
};

class Asteroids : public GameBase
{
public:
    void InitGame() override;
    void UpdateGame(float deltaTime) override;
    void DrawGame() override;
    void UnloadGame() override;

private:
    GameObjectRect* m_player = nullptr;
    Bullet* m_bullets = nullptr;

    float m_screenWidth = 0;
    float m_screenHeight = 0;

    Vector2 m_lastMovementDirection = {0, 0};
    float m_currentSpeed = 0;
    bool m_isMoving = false;

    Vector2 GetMovementDirection() const;
    void ShootBullet(Vector2 position, Vector2 direction);
};
