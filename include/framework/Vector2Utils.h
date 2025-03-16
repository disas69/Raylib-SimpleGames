#ifndef VECTOR2UTILS_H
#define VECTOR2UTILS_H

#include <cmath>
#include <raylib.h>

namespace Vector2Utils
{
    inline Vector2 GetRandom(const float minX, const float maxX, const float minY, const float maxY)
    {
        constexpr float multiplier = 1000.f;
        const float x = GetRandomValue(minX * multiplier, maxX * multiplier) / multiplier;
        const float y = GetRandomValue(minY * multiplier, maxY * multiplier) / multiplier;
        return {x, y};
    }

    inline Vector2 Normalize(const Vector2 vector)
    {
        float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
        return {vector.x / length, vector.y / length};
    }

    inline float DotProduct(const Vector2 a, const Vector2 b)
    {
        return a.x * b.x + a.y * b.y;
    }

    inline Vector2 Scale(const Vector2 vector, const float scalar)
    {
        return {vector.x * scalar, vector.y * scalar};
    }

    inline Vector2 Subtract(const Vector2 a, const Vector2 b)
    {
        return {a.x - b.x, a.y - b.y};
    }

    inline Vector2 Reflect(const Vector2 vector, const Vector2 normal)
    {
        auto [x, y] = Scale(normal, 2 * DotProduct(vector, normal));
        return {vector.x - x, vector.y - y};
    }
}  // namespace Vector2Utils

#endif  // VECTOR2UTILS_H
