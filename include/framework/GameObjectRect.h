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

    Rectangle GetRectangle() const { return {m_position.x, m_position.y, static_cast<float>(m_width), static_cast<float>(m_height)}; }

private:
    int m_width = 0;
    int m_height = 0;
};
