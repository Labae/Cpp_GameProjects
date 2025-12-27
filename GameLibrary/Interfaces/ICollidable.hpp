#pragma once

namespace GameLibrary
{
    struct Bounds
    {
        int32_t x = 0;
        int32_t y = 0;
        int32_t width = 0;
        int32_t height = 0;
    };

    class ICollidable
    {
    public:
        virtual ~ICollidable() = default;

        virtual Bounds GetBounds() const = 0;
        virtual void OnCollision(ICollidable* other) = 0;
    };
}