//
// Created by Labae on 3/1/26.
//

#ifndef BOARD_HPP
#define BOARD_HPP
#include "Components/Component.hpp"
#include "Configs/TetrisConfig.hpp"

#include <vector>

namespace Tetris
{
    class Board final : public GameLibrary::Component
    {
    public:
        Board(GameLibrary::Actor* owner, const TetrisConfig& config);

        void Render(GameLibrary::IGraphics& graphics) override;
        void Clear();

        [[nodiscard]] int32_t ClearFullLines();

        [[nodiscard]] bool IsCellEmpty(int32_t x, int32_t y) const noexcept;
        void SetCell(int32_t x, int32_t y, uint8_t value) noexcept;

    private:
        void RenderGrid(GameLibrary::IGraphics& graphics) const;
        void RenderBorder(GameLibrary::IGraphics& graphics) const;
        void RenderCells(GameLibrary::IGraphics& graphics) const;

        const TetrisConfig& m_config;
        std::vector<std::vector<uint8_t>> m_cells;

        int32_t m_boardPixelWidth{};
        int32_t m_boardPixelHeight{};
    };
} // namespace Tetris

#endif // BOARD_HPP
