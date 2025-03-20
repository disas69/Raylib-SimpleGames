#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <raylib.h>

class GameObject2D
{
public:
    GameObject2D() : position({0.f, 0.f}), color{WHITE} {}
    explicit GameObject2D(const Vector2 pos) : position(pos), color{WHITE} {}
    explicit GameObject2D(const Color& color) : position({0.f, 0.f}), color(color) {}
    GameObject2D(const Vector2 position, const Color& color) : position({0.f, 0.f}), color(color) {}

    virtual void Draw() const = 0;
    virtual ~GameObject2D() = default;

    void SetPosition(const Vector2 position)
    {
        this->position = position;
    }

    Vector2 GetPosition() const
    {
        return position;
    }

    float GetRotation() const
    {
        return rotation;
    }

    void SetRotation(const float rotation)
    {
        this->rotation = rotation;
    }

    void SetColor(const Color& color)
    {
        this->color = color;
    }

    Color GetColor() const
    {
        return color;
    }

protected:
    Vector2 position = {0.f, 0.f};
    float rotation = 0;
    Color color;
};

#endif //GAMEOBJECT_H
