//
// Created by Labae on 3/14/26.
//

#include "Components/NextQueue.hpp"

#include "Data/Cell.hpp"
#include "Interfaces/IGraphics.hpp"
#include "Systems/Logger.hpp"

namespace
{
    constexpr sf::Color COLOR_BOX_BG{30, 30, 30, 255};
    constexpr sf::Color COLOR_BOX_BORDER{100, 100, 100, 255};
    constexpr int32_t BOX_SPACING = 10;
} // namespace

namespace Tetris
{
    NextQueue::NextQueue(GameLibrary::Actor* owner, const TetrisConfig& config, int32_t previewCount)
        : Component(owner)
        , m_config(config)
        , m_previewCount(previewCount)
    {
        FillQueue();
    }

    void NextQueue::Render(GameLibrary::IGraphics& graphics)
    {
        const int32_t boxSize = m_config.nextBoxSize;
        const int32_t cellSize = boxSize / m_previewCount;
        const int32_t spacing = boxSize + BOX_SPACING;
        ;

        // 라벨
        graphics.DrawLabel("NEXT", m_config.nextBoxX + boxSize / 2, m_config.nextBoxY - 25, 16, sf::Color::White,
                           GameLibrary::TextAlign::Center);

        for (int32_t i = 0; i < m_previewCount && i < static_cast<int32_t>(m_queue.size()); ++i)
        {
            const int32_t boxY = m_config.nextBoxY + i * spacing;

            // 박스 배경
            graphics.FillRect(m_config.nextBoxX, boxY, boxSize, boxSize, COLOR_BOX_BG);
            graphics.DrawRect(m_config.nextBoxX, boxY, boxSize, boxSize, COLOR_BOX_BORDER);

            // 피스 렌더링
            const auto& tetromino = GetTetromino(m_queue[i]);
            const int32_t offsetX = m_config.nextBoxX + m_config.nextBoxPadding;
            const int32_t offsetY = boxY + m_config.nextBoxPadding;

            for (const auto& [col, row] : tetromino.GetBlockPositions(0))
            {
                const int32_t px = offsetX + col * cellSize;
                const int32_t py = offsetY + row * cellSize;
                Cell(m_queue[i]).Render(graphics, px, py, cellSize);
            }
        }
    }

    ETetromino NextQueue::Pop()
    {
        const ETetromino next = m_queue.front();
        m_queue.pop_front();
        FillQueue();
        return next;
    }

    ETetromino NextQueue::Peek() const
    {
        return m_queue.front();
    }

    void NextQueue::FillQueue()
    {
        while (static_cast<int32_t>(m_queue.size()) < m_previewCount + 1)
        {
            if (m_bag.empty())
            {
                FillBag();
            }

            m_queue.emplace_back(m_bag.back());
            m_bag.pop_back();
        }
    }

    void NextQueue::FillBag()
    {
        m_bag.clear();

        for (auto i = 0; i < static_cast<uint8_t>(ETetromino::Count); ++i)
        {
            m_bag.emplace_back(static_cast<ETetromino>(i));
        }

        std::ranges::shuffle(m_bag, m_rng);
    }
} // namespace Tetris