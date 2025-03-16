#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <raylib.h>

class GameObject2D
{
public:
    GameObject2D() : transform{0.f, 0.f, 0.f}, color{WHITE} {}
    explicit GameObject2D(const Vector2& position) : transform{position.x, position.y, 0.f}, color{WHITE} {}
    explicit GameObject2D(const Color& color) : transform{0.f, 0.f, 0.f}, color(color) {}
    GameObject2D(const Vector2& position, const Color& color) : transform{position.x, position.y, 0.f}, color(color) {}

    virtual void Draw() const = 0;
    virtual ~GameObject2D() = default;

    void SetPosition(const Vector2& position)
    {
        transform.translation.x = position.x;
        transform.translation.y = position.y;
    }

    Vector2 GetPosition() const
    {
        return {transform.translation.x, transform.translation.y};
    }

    Quaternion GetRotation() const
    {
        return transform.rotation;
    }

    void SetRotation(const Quaternion& rotation)
    {
        transform.rotation = rotation;
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
    Transform transform;
    Color color;
};

#endif //GAMEOBJECT_H
