#pragma once

#include <SFML/System/Vector2.hpp>
#include <cstdint>

struct SnakeMovedEvent
{
    sf::Vector2f position{};
};

struct FoodEatenEvent
{
    float x{};
    float y{};
};

struct GoldenFoodEatenEvent
{
    float x{};
    float y{};
};

struct GameOverEvent
{
    int32_t score{};
};

struct ScoreChangedEvent
{
    int32_t score{};
};