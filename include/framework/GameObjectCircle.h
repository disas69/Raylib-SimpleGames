#pragma once

#include "GameObject2D.h"

class GameObjectCircle : public GameObject2D
{
public:
    GameObjectCircle() : m_radius(0) {}
    GameObjectCircle(const Color& color, const float radius) : GameObject2D(color), m_radius(radius) {}
    explicit GameObjectCircle(const float radius) : m_radius(radius) {}
    GameObjectCircle(const Vector2& position, const Color& color, const float radius) : GameObject2D(position, color), m_radius(radius) {}

    void Draw() const override
    {
        if (!m_isActive)
        {
            return;
        }

        DrawCircleV(m_position, m_radius, m_color);
    }

    void SetRadius(const float radius) { this->m_radius = radius; }
    float GetRadius() const { return m_radius; }

private:
    float m_radius;
};
