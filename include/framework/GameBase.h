#pragma once

class GameBase
{
public:
    GameBase() = default;
    virtual ~GameBase() = default;
    virtual void InitGame() = 0;
    virtual void UpdateGame(float deltaTime) = 0;
    virtual void DrawGame() = 0;
    virtual void UnloadGame() = 0;
};
