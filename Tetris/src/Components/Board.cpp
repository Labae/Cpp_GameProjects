#include "Components/Board.hpp"

#include "Config/TetrisConfig.hpp"
#include "Data/Tetromino.hpp"
#include "Interfaces/IGraphics.hpp"

#include <algorithm>

namespace
{
    constexpr sf::Color COLOR_GRID{40, 40, 40, 255};
    constexpr sf::Color COLOR_BACKGROUND{20, 20, 20, 255};
} // namespace

namespace Tetris
{
    Board::Board(GameLibrary::Actor* owner, const TetrisConfig& config)
        : Component(owner)
        , m_config(config)
        , m_cells(config.boardHeight, std::vector<Cell>(config.boardWidth))
        , m_boardPixelWidth(config.cellSize * config.boardWidth)
        , m_boardPixelHeight(config.cellSize * config.boardHeight)
    {
    }

    void Board::Render(GameLibrary::IGraphics& graphics)
    {
        graphics.FillRect(m_config.boardX, m_config.boardY,
                          m_boardPixelWidth, m_boardPixelHeight, COLOR_BACKGROUND);

        RenderCells(graphics);
        RenderGrid(graphics);
    }

    void Board::Clear()
    {
        for (auto& row : m_cells)
        {
            std::ranges::fill(row, Cell{});
        }
    }

    bool Board::IsCellEmpty(const int32_t x, const int32_t y) const noexcept
    {
        if (x < 0 || x >= m_config.boardWidth || y < 0 || y >= m_config.boardHeight)
        {
            return false;
        }
        return m_cells[y][x].IsEmpty();
    }

    void Board::SetCell(const int32_t x, const int32_t y, const Cell& cell) noexcept
    {
        if (x >= 0 && x < m_config.boardWidth && y >= 0 && y < m_config.boardHeight)
        {
            m_cells[y][x] = cell;
        }
    }

    int32_t Board::ClearFullLines()
    {
        int32_t linesCleared = 0;

        for (int32_t y = m_config.boardHeight - 1; y >= 0; --y)
        {
            bool isFull = true;
            for (int32_t x = 0; x < m_config.boardWidth; ++x)
            {
                if (m_cells[y][x].IsEmpty())
                {
                    isFull = false;
                    break;
                }
            }

            if (isFull)
            {
                ++linesCleared;

                for (int32_t row = y; row > 0; --row)
                {
                    m_cells[row] = m_cells[row - 1];
                }

                std::ranges::fill(m_cells[0], Cell{});
                ++y;
            }
        }

        return linesCleared;
    }

    bool Board::CanSpawnAt(const ETetromino type) const noexcept
    {
        const int32_t spawnX = (m_config.boardWidth - 4) / 2;

        const auto& tetromino = GetTetromino(type);
        for (const auto& [col, row] : tetromino.GetBlockPositions(0))
        {
            if (not IsCellEmpty(spawnX + col, row))
            {
                return false;
            }
        }

        return true;
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

    void Board::RenderCells(GameLibrary::IGraphics& graphics) const
    {
        for (int32_t y = 0; y < m_config.boardHeight; ++y)
        {
            for (int32_t x = 0; x < m_config.boardWidth; ++x)
            {
                const int32_t px = m_config.boardX + x * m_config.cellSize;
                const int32_t py = m_config.boardY + y * m_config.cellSize;
                m_cells[y][x].Render(graphics, px, py, m_config.cellSize);
            }
        }
    }
} // namespace Tetris