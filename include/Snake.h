#pragma once

#include <framework/GameBase.h>

class GameObjectRect;
class GameObjectCircle;

class Snake : public GameBase
{
public:
    explicit Snake(ArenaAllocator& arena);
    void UpdateGame(float deltaTime) override;
    void DrawGame() override;

private:
    GameObjectCircle* m_player = nullptr;
    GameObjectCircle* m_trail = nullptr;
    GameObjectRect* m_pickup = nullptr;

    int m_trailCount = 0;
    int m_score = 0;
    float m_time = 0;
    int m_startDelay = 1;
    float m_startTimer = 0;
    bool m_isGameOver = false;

    Vector2 GetRandomPosition() const;
    void ResetGame();
    void CollectPickup();
    void GameOver();
};