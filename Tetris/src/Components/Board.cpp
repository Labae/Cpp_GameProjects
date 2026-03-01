//
// Created by Labae on 3/1/26.
//

#include "Components/Board.hpp"

#include "Interfaces/IGraphics.hpp"

namespace
{
    constexpr sf::Color COLOR_BACKGROUND{20, 20, 20, 255};
    constexpr sf::Color COLOR_GRID{40, 40, 40, 255};
    constexpr sf::Color COLOR_BORDER{100, 100, 100, 255};
} // namespace

namespace Tetris
{
    Board::Board(GameLibrary::Actor* owner, const TetrisConfig& config)
        : Component(owner)
        , m_config(config)
        , m_cells(config.boardHeight, std::vector<uint8_t>(config.boardWidth, 0))
        , m_boardPixelWidth(config.cellSize * config.boardWidth)
        , m_boardPixelHeight(config.cellSize * config.boardHeight)
    {
    }

    void Board::Render(GameLibrary::IGraphics& graphics)
    {
        graphics.FillRect(m_config.boardX, m_config.boardY, m_boardPixelWidth, m_boardPixelHeight, COLOR_BACKGROUND);

        RenderGrid(graphics);
        RenderBorder(graphics);
    }

    void Board::Clear()
    {
        for (auto& row : m_cells)
        {
            std::ranges::fill(row, 0);
        }
    }
    bool Board::IsCellEmpty(int32_t x, int32_t y) const
    {
        if (x < 0 || x >= m_config.boardWidth || y < 0 || y >= m_config.boardHeight)
        {
            return false;
        }

        return m_cells[y][x] == 0;
    }

    void Board::RenderGrid(GameLibrary::IGraphics& graphics) const
    {
        for (int32_t x = 0; x <= m_config.boardWidth; ++x)
        {
            const int32_t px = m_config.boardX + x * m_config.cellSize;
            graphics.DrawLine(px, m_config.boardY, px, m_config.boardY + m_boardPixelHeight, COLOR_GRID);
        }

        for (int32_t y = 0; y <= m_config.boardHeight; ++y)
        {
            const int32_t py = m_config.boardY + y * m_config.cellSize;
            graphics.DrawLine(m_config.boardX, py, m_config.boardX + m_boardPixelWidth, py, COLOR_GRID);
        }
    }

    void Board::RenderBorder(GameLibrary::IGraphics& graphics) const
    {
        graphics.DrawRect(m_config.boardX, m_config.boardY, m_boardPixelWidth, m_boardPixelHeight, COLOR_BORDER);
    }
} // namespace Tetris