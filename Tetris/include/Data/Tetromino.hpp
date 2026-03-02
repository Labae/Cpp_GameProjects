#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <cstdint>

namespace Tetris
{
    enum class ETetromino : uint8_t
    {
        I, O, T, S, Z, J, L,
        Count
    };

    struct Tetromino
    {
        ETetromino type{};
        sf::Color color{};
        std::array<std::array<std::array<uint8_t, 4>, 4>, 4> shapes{};

        [[nodiscard]] std::array<sf::Vector2i, 4> GetBlockPositions(int32_t rotation) const noexcept;
    };

    [[nodiscard]] const Tetromino& GetTetromino(ETetromino type) noexcept;
} // namespace Tetris