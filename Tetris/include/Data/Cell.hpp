#pragma once

#include <SFML/Graphics/Color.hpp>
#include <cstdint>

namespace GameLibrary
{
    class IGraphics;
}

namespace Tetris
{
    enum class ETetromino : uint8_t;

    class Cell
    {
    public:
        Cell() = default;
        explicit Cell(ETetromino tetromino);
        explicit Cell(const sf::Color& color);

        [[nodiscard]] bool IsEmpty() const noexcept { return not m_filled; }
        [[nodiscard]] bool IsFilled() const noexcept { return m_filled; }

        void Render(GameLibrary::IGraphics& graphics, int32_t x, int32_t y, int32_t size) const;

        [[nodiscard]] static Cell Garbage() noexcept;

    private:
        sf::Color m_color{};
        bool m_filled{};
    };
} // namespace Tetris