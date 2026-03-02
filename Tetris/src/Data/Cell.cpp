#include "Data/Cell.hpp"

#include "Data/Tetromino.hpp"
#include "Interfaces/IGraphics.hpp"

namespace
{
    constexpr sf::Color COLOR_BORDER{0, 0, 0, 255};
    constexpr sf::Color COLOR_GARBAGE{128, 128, 128, 255};
} // namespace

namespace Tetris
{
    Cell::Cell(const ETetromino tetromino)
        : m_color(GetTetromino(tetromino).color)
        , m_filled(true)
    {
    }

    Cell::Cell(const sf::Color& color)
        : m_color(color)
        , m_filled(true)
    {
    }

    Cell Cell::Garbage() noexcept
    {
        return Cell(COLOR_GARBAGE);
    }

    void Cell::Render(GameLibrary::IGraphics& graphics, const int32_t x, const int32_t y, const int32_t size) const
    {
        if (not m_filled)
        {
            return;
        }

        graphics.FillRect(x, y, size, size, m_color);
        graphics.DrawRect(x, y, size, size, COLOR_BORDER);
    }
} // namespace Tetris