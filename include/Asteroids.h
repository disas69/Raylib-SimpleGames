#pragma once

#include "framework/GameBase.h"

class GameObjectRect;

class Asteroids : public GameBase
{
public:
    void InitGame() override;
    void UpdateGame(float deltaTime) override;
    void DrawGame() override;
    void UnloadGame() override;

private:
    GameObjectRect* m_player = nullptr;

    float m_screenWidth = 0;
    float m_screenHeight = 0;
};
