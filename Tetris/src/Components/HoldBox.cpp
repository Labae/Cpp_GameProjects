#include "Components/HoldBox.hpp"

#include "Config/TetrisConfig.hpp"
#include "Data/Cell.hpp"
#include "Interfaces/IGraphics.hpp"

namespace
{
    constexpr sf::Color COLOR_BOX_BG{30, 30, 30, 255};
    constexpr sf::Color COLOR_BOX_BORDER{100, 100, 100, 255};
    constexpr sf::Color COLOR_HOLD_DISABLED{100, 100, 100, 128};
} // namespace

namespace Tetris
{
    HoldBox::HoldBox(GameLibrary::Actor* owner, const TetrisConfig& config)
        : Component(owner)
        , m_config(config)
    {
    }

    void HoldBox::Render(GameLibrary::IGraphics& graphics)
    {
        // 박스 배경
        graphics.FillRect(m_config.holdBoxX, m_config.holdBoxY,
                          m_config.holdBoxSize, m_config.holdBoxSize, COLOR_BOX_BG);
        graphics.DrawRect(m_config.holdBoxX, m_config.holdBoxY,
                          m_config.holdBoxSize, m_config.holdBoxSize, COLOR_BOX_BORDER);

        // 라벨
        graphics.DrawLabel("HOLD", m_config.holdBoxX + m_config.holdBoxSize / 2,
                           m_config.holdBoxY - 25, 16, sf::Color::White,
                           GameLibrary::TextAlign::Center);

        if (not m_holdPiece.has_value())
        {
            return;
        }

        // 홀드된 피스 렌더링
        const auto& tetromino = GetTetromino(m_holdPiece.value());
        const sf::Color color = m_canHold ? tetromino.color : COLOR_HOLD_DISABLED;
        const int32_t cellSize = m_config.holdBoxSize / 5;
        const int32_t offsetX = m_config.holdBoxX + m_config.holdBoxPadding;
        const int32_t offsetY = m_config.holdBoxY + m_config.holdBoxPadding;

        for (const auto& [col, row] : tetromino.GetBlockPositions(0))
        {
            const int32_t px = offsetX + col * cellSize;
            const int32_t py = offsetY + row * cellSize;
            Cell(color).Render(graphics, px, py, cellSize);
        }
    }

    std::optional<ETetromino> HoldBox::Hold(const ETetromino current)
    {
        if (not m_canHold)
        {
            return std::nullopt;
        }

        m_canHold = false;

        if (m_holdPiece.has_value())
        {
            const ETetromino previous = m_holdPiece.value();
            m_holdPiece = current;
            return previous;
        }

        m_holdPiece = current;
        return std::nullopt;
    }
} // namespace Tetris