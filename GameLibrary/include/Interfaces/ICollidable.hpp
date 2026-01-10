#pragma once

#include <SFML/Graphics/Rect.hpp>

namespace GameLibrary
{
    class Actor;

    class ICollidable
    {
    public:
        virtual ~ICollidable() = default;

        [[nodiscard]] virtual sf::IntRect GetBounds() const = 0;
        [[nodiscard]] virtual Actor* GetActor() const = 0;
        virtual void OnCollision(Actor* other) = 0;
    };
} // namespace GameLibrary