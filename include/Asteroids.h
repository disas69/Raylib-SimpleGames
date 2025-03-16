#ifndef ASTEROIDS_H
#define ASTEROIDS_H

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
    GameObjectRect* player = nullptr;

    float screenWidth = 0;
    float screenHeight = 0;
};

#endif //ASTEROIDS_H
