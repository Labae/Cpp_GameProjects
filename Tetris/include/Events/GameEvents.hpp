#pragma once

#include <cstdint>

namespace Tetris
{
    struct PieceLockedEvent {};

    struct GameOverEvent {};

    struct LinesClearedEvent
    {
        int32_t lineCount{};
    };

    struct QuitEvent{};
} // namespace Tetris