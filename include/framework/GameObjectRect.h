#pragma once

#include "GameObject2D.h"
#include <raylib.h>

class GameObjectRect : public GameObject2D
{
public:
    GameObjectRect() {}
    GameObjectRect(const Color& color, const int width, const int height) : GameObject2D(color), m_width(width), m_height(height) {}
    GameObjectRect(const int width, const int height) : m_width(width), m_height(height) {}
    GameObjectRect(const Vector2& position, const Color& color, const int width, const int height) : GameObject2D(position, color), m_width(width), m_height(height) {}

    void Draw() const override { DrawRectanglePro(GetRectangle(), {0.f, 0.f}, m_rotation, m_color); }

    Rectangle GetRectangle() const
    {
        const float width = static_cast<float>(m_width);
        const float height = static_cast<float>(m_height);
        return {m_position.x - width / 2, m_position.y - height / 2, width, height};
    }

private:
    int m_width = 0;
    int m_height = 0;
};
