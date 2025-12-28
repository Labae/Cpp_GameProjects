#pragma once

#include <SFML/Graphics/Rect.hpp>

namespace GameLibrary
{
    class ICollidable
    {
    public:
        virtual ~ICollidable() = default;

        [[nodiscard]] virtual sf::IntRect GetBounds() const = 0;
        virtual void OnCollision(ICollidable* other) = 0;
    };
} // namespace GameLibrary