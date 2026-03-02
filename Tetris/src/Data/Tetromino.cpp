#include "Data/Tetromino.hpp"

namespace
{
    using namespace Tetris;

    constexpr sf::Color COLOR_I{0, 255, 255, 255};
    constexpr sf::Color COLOR_O{255, 255, 0, 255};
    constexpr sf::Color COLOR_T{128, 0, 128, 255};
    constexpr sf::Color COLOR_S{0, 255, 0, 255};
    constexpr sf::Color COLOR_Z{255, 0, 0, 255};
    constexpr sf::Color COLOR_J{0, 0, 255, 255};
    constexpr sf::Color COLOR_L{255, 165, 0, 255};

    constexpr Tetromino TETROMINO_I{
        .type = ETetromino::I,
        .color = COLOR_I,
        .shapes = {{
            {{{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}}},
            {{{0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0}}},
            {{{0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {0,0,0,0}}},
            {{{0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0}}}
        }}
    };

    constexpr Tetromino TETROMINO_O{
        .type = ETetromino::O,
        .color = COLOR_O,
        .shapes = {{
            {{{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}},
            {{{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}},
            {{{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}},
            {{{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}}
        }}
    };

    constexpr Tetromino TETROMINO_T{
        .type = ETetromino::T,
        .color = COLOR_T,
        .shapes = {{
            {{{0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}},
            {{{0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}}},
            {{{0,0,0,0}, {1,1,1,0}, {0,1,0,0}, {0,0,0,0}}},
            {{{0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}}}
        }}
    };

    constexpr Tetromino TETROMINO_S{
        .type = ETetromino::S,
        .color = COLOR_S,
        .shapes = {{
            {{{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}},
            {{{0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0}}},
            {{{0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0}}},
            {{{1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}}}
        }}
    };

    constexpr Tetromino TETROMINO_Z{
        .type = ETetromino::Z,
        .color = COLOR_Z,
        .shapes = {{
            {{{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}},
            {{{0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}}},
            {{{0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0}}},
            {{{0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0}}}
        }}
    };

    constexpr Tetromino TETROMINO_J{
        .type = ETetromino::J,
        .color = COLOR_J,
        .shapes = {{
            {{{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}},
            {{{0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}}},
            {{{0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0}}},
            {{{0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0}}}
        }}
    };

    constexpr Tetromino TETROMINO_L{
        .type = ETetromino::L,
        .color = COLOR_L,
        .shapes = {{
            {{{0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}},
            {{{0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0}}},
            {{{0,0,0,0}, {1,1,1,0}, {1,0,0,0}, {0,0,0,0}}},
            {{{1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}}}
        }}
    };

    constexpr std::array<Tetromino, 7> TETROMINOES = {
        TETROMINO_I, TETROMINO_O, TETROMINO_T, TETROMINO_S,
        TETROMINO_Z, TETROMINO_J, TETROMINO_L
    };
} // namespace

namespace Tetris
{
    std::array<sf::Vector2i, 4> Tetromino::GetBlockPositions(const int32_t rotation) const noexcept
    {
        std::array<sf::Vector2i, 4> positions{};
        size_t index = 0;

        for (int32_t row = 0; row < 4; ++row)
        {
            for (int32_t col = 0; col < 4; ++col)
            {
                if (shapes[rotation][row][col] == 1)
                {
                    positions[index++] = {col, row};
                }
            }
        }

        return positions;
    }

    const Tetromino& GetTetromino(const ETetromino type) noexcept
    {
        return TETROMINOES[static_cast<size_t>(type)];
    }
} // namespace Tetris