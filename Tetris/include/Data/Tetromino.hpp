//
// Created by Labae on 3/1/26.
//

#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include <array>
#include <SFML/Graphics/Color.hpp>

namespace Tetris
{
    enum class ETetromino : uint8_t
    {
        I,
        O,
        T,
        S,
        Z,
        J,
        L,
        Count
    };

    struct Tetromino
    {
        ETetromino type{};
        sf::Color color{};
        std::array<std::array<std::array<uint8_t, 4>, 4>, 4> shapes{};
    };

    [[nodiscard]] const Tetromino& GetTetromino(ETetromino type) noexcept;
    [[nodiscard]] const sf::Color& GetTetrominoColor(ETetromino type) noexcept;
} // namespace Tetris

#endif // TETROMINO_HPP
