//
// Created by Labae on 3/1/26.
//

#ifndef TETRISCONFIG_HPP
#define TETRISCONFIG_HPP
#include <cstdint>

namespace Tetris
{
    struct TetrisConfig
    {
        // 보드 설정
        int32_t boardWidth{10};
        int32_t boardHeight{20};
        int32_t cellSize{32};

        // 레이아웃
        int32_t boardX{240};    // (800 - 320) / 2
        int32_t boardY{80};

        // Hold/Next 박스
        int32_t holdNextBoxSize{128};   // 4셀 * 32px
    };
}

#endif //TETRISCONFIG_HPP
