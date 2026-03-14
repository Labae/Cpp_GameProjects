#pragma once

#include <cstdint>
#include <array>

namespace Tetris
{
    struct TetrisConfig
    {
        // 보드
        int32_t boardWidth{10};
        int32_t boardHeight{20};
        int32_t cellSize{32};
        int32_t boardX{240};
        int32_t boardY{80};

        // Hold 박스
        int32_t holdBoxX{60};
        int32_t holdBoxY{100};
        int32_t holdBoxSize{128};
        int32_t holdBoxPadding{10};

        // Next 박스
        int32_t nextBoxX{600};
        int32_t nextBoxY{100};
        int32_t nextBoxSize{128};
        int32_t nextBoxPadding{10};

        // 게임플레이
        float fallInterval{1.0f};

        // 점수
        std::array<int32_t, 4> lineScores{100, 300, 500, 800};
    };
} // namespace Tetris