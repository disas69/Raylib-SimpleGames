#pragma once

#include "framework/GameBase.h"
#include "framework/GameObjectCircle.h"

class GameObjectRect;

class Bullet : public GameObjectCircle
{
public:
    Bullet() = default;
    void Init(const Color& color, float radius, float screenWidth, float screenHeight);
    void Spawn(Vector2 position, Vector2 direction);
    void Update(float deltaTime);

private:
    Vector2 m_direction = {0, 0};

    float m_screenWidth = 0;
    float m_screenHeight = 0;
};

class Rock : public GameObjectCircle
{
public:
    Rock() = default;
    void Init(const Color& color, float screenWidth, float screenHeight);
    void Spawn(int size, Vector2 position, Vector2 direction);
    void Update(float deltaTime);
    void Hit();

    inline int GetSize() const { return m_size; }

private:
    Vector2 m_direction = {0, 0};

    float m_screenWidth = 0;
    float m_screenHeight = 0;

    int m_size = 0;
};

class Asteroids : public GameBase
{
public:
    Asteroids(ArenaAllocator& arena);
    void UpdateGame(float deltaTime) override;
    void DrawGame() override;

private:
    GameObjectRect* m_player = nullptr;
    Bullet* m_bullets = nullptr;
    Rock* m_rocks = nullptr;

    float m_time = 0;
    float m_lastSpawnTime = 0;
    int m_score = 0;
    int m_startDelay = 1;
    float m_startTimer = 0;

    Vector2 m_lastMovementDirection = {0, 0};
    float m_currentSpeed = 0;
    bool m_isMoving = false;
    bool m_isGameOver = false;

    Vector2 GetMovementDirection() const;
    void SpawnBullet(Vector2 position, Vector2 direction);
    void SpawnRock();
    void GameOver();
    void ResetGame();
};
