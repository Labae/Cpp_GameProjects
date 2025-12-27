#pragma once

#include "Component.hpp"

namespace GameLibrary
{
    struct Vector2
    {
        float x = 0.0f;
        float y = 0.0f;

        Vector2 operator+(const Vector2& other) const noexcept { return { x + other.x, y + other.y }; }
        Vector2 operator-(const Vector2& other) const noexcept { return { x - other.x, y - other.y }; }
        Vector2 operator*(float scalar) const noexcept { return { x * scalar, y * scalar }; }
        bool operator==(const Vector2& other) const noexcept = default;
    };

    class Transform : public Component
    {
    public:
        Vector2 position{};
        Vector2 scale = { 1.0f, 1.0f };
        float rotation{};
    };
}