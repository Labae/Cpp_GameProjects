#pragma once

#include "Component.hpp"

#include <SFML/System/Vector2.hpp>

namespace GameLibrary
{
    class Transform final : public Component
    {
    public:
    explicit Transform(Actor* owner) : Component(owner) {}

        sf::Vector2f position{};
        sf::Vector2f scale = {1.0f, 1.0f};
        float rotation{};
    };
} // namespace GameLibrary