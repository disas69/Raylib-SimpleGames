#ifndef GAMEOBJECTCIRCLE_H
#define GAMEOBJECTCIRCLE_H

#include "GameObject2D.h"

class GameObjectCircle : public GameObject2D
{
public:
    GameObjectCircle() : GameObject2D(), radius(0) {}
    GameObjectCircle(const Color& color, const float radius) : GameObject2D(color), radius(radius) {}
    GameObjectCircle(const float radius) : GameObject2D(), radius(radius) {}
    GameObjectCircle(const Vector2& position, const Color& color, const float radius) : GameObject2D(position, color), radius(radius) {}

    void Draw() const override
    {
        DrawCircleV({position.x, position.y}, radius, color);
    }

    float GetRadius() const
    {
        return radius;
    }

private:
    float radius;
};

#endif //GAMEOBJECTCIRCLE_H
