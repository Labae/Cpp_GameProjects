#pragma once

#include "GameObject/Transform.hpp"

struct SnakeMovedEvent
{
    GameLibrary::Vector2 position;
};

struct FoodEatenEvent
{
    float x;
    float y;
};

struct GoldenFoodEatenEvent
{
    float x;
    float y;
};

struct GameOverEvent
{
    int score;
};

struct ScoreChangedEvent
{
    int score;
};

