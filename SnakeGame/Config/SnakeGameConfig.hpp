#pragma once

#include "Interfaces/IGraphics.hpp"

namespace SnakeGame
{
    struct SnakeGameConfig
    {
        int gridSize = 20;
        float moveInterval = 0.1f;
        float gameOverDelay = 1.0f;

        GameLibrary::Color headColor = { 0, 255, 0, 255 };
        GameLibrary::Color tailColor = { 0, 100, 0, 255 };
    };
}