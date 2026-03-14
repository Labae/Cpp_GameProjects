#pragma once

#include "Components/Component.hpp"
#include "Data/Cell.hpp"

#include <cstdint>
#include <vector>

namespace Tetris
{
    struct TetrisConfig;

    class Board : public GameLibrary::Component
    {
    public:
        Board(GameLibrary::Actor* owner, const TetrisConfig& config);

        void Render(GameLibrary::IGraphics& graphics) override;

        void Clear();
        [[nodiscard]] bool IsCellEmpty(int32_t x, int32_t y) const noexcept;
        void SetCell(int32_t x, int32_t y, const Cell& cell) noexcept;
        [[nodiscard]] int32_t ClearFullLines();

        [[nodiscard]] bool CanSpawnAt(ETetromino type) const noexcept;

    private:
        void RenderGrid(GameLibrary::IGraphics& graphics) const;
        void RenderCells(GameLibrary::IGraphics& graphics) const;

        const TetrisConfig& m_config;
        std::vector<std::vector<Cell>> m_cells;

        int32_t m_boardPixelWidth{};
        int32_t m_boardPixelHeight{};
    };
} // namespace Tetris