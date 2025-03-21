#pragma once

#include "GameObject2D.h"

class GameObjectCircle : public GameObject2D
{
public:
    GameObjectCircle() : m_radius(0) {}
    GameObjectCircle(const Color& color, const float radius) : GameObject2D(color), m_radius(radius) {}
    explicit GameObjectCircle(const float radius) : m_radius(radius) {}
    GameObjectCircle(const Vector2& position, const Color& color, const float radius) : GameObject2D(position, color), m_radius(radius) {}

    void Draw() const override { DrawCircleV({m_position.x, m_position.y}, m_radius, m_color); }

    float GetRadius() const { return m_radius; }

private:
    float m_radius;
};
