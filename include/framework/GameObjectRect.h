#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject2D.h"
#include <raylib.h>

class GameObjectRect : public GameObject2D
{
public:
    GameObjectRect() : GameObject2D(), width(0), height(0) {}
    GameObjectRect(const Color& color, const int width, const int height) : GameObject2D(color), width(width), height(height) {}
    GameObjectRect(const int width, const int height) : GameObject2D(), width(width), height(height) {}
    GameObjectRect(const Vector2& position, const Color& color, const int width, const int height) : GameObject2D(position, color), width(width), height(height) {}

    void Draw() const override
    {
        DrawRectangleRec(GetRectangle(), color);
    }

    Rectangle GetRectangle() const
    {
        return {transform.translation.x, transform.translation.y, static_cast<float>(width), static_cast<float>(height)};
    }

private:
    int width;
    int height;
};

#endif  // PLAYER_H
