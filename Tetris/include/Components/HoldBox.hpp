#pragma once

#include "Components/Component.hpp"
#include "Data/Tetromino.hpp"

#include <cstdint>
#include <optional>

namespace Tetris
{
    struct TetrisConfig;

    class HoldBox : public GameLibrary::Component
    {
    public:
        HoldBox(GameLibrary::Actor* owner, const TetrisConfig& config);

        void Render(GameLibrary::IGraphics& graphics) override;

        [[nodiscard]] std::optional<ETetromino> Hold(ETetromino current);
        void ResetCanHold() noexcept { m_canHold = true; }
        [[nodiscard]] bool CanHold() const noexcept { return m_canHold; }

    private:
        const TetrisConfig& m_config;
        std::optional<ETetromino> m_holdPiece{};
        bool m_canHold{true};
    };
} // namespace Tetris