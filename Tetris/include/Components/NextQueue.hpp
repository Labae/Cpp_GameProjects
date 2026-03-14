//
// Created by Labae on 3/14/26.
//

#ifndef NEXTQUEUE_HPP
#define NEXTQUEUE_HPP

#include "Components/Component.hpp"
#include "Config/TetrisConfig.hpp"
#include "Data/Tetromino.hpp"

#include <queue>

namespace Tetris
{
    class NextQueue final : public GameLibrary::Component
    {
    public:
        explicit NextQueue(GameLibrary::Actor* owner, const TetrisConfig& config, int32_t previewCount = 5);

        void Render(GameLibrary::IGraphics& graphics) override;

        [[nodiscard]] ETetromino Pop();
        [[nodiscard]] ETetromino Peek() const;

    private:
        void FillQueue();
        void FillBag();

        const TetrisConfig& m_config;

        int32_t m_previewCount{};
        std::deque<ETetromino> m_queue{};
        std::vector<ETetromino> m_bag{};
    };
}

#endif //NEXTQUEUE_HPP
