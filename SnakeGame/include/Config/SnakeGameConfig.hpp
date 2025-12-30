#pragma once

#include <SFML/Graphics/Color.hpp>
#include <cstdint>

namespace SnakeGame
{
    struct SnakeGameConfig
    {
        int32_t gridSize{20};
        float moveInterval{0.1f};
        float gameOverDelay{1.0f};

        sf::Color headColor{0, 255, 0, 255};
        sf::Color tailColor{0, 100, 0, 255};
    };
} // namespace SnakeGame