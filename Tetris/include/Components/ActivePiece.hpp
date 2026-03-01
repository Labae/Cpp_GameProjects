//
// Created by Labae on 3/1/26.
//

#ifndef ACTIVEPIECE_HPP
#define ACTIVEPIECE_HPP
#include "Components/Component.hpp"
#include "Data/Tetromino.hpp"
#include "Interfaces/IInputProvider.hpp"

namespace Tetris
{
    struct TetrisConfig;
    class Board;

    class ActivePiece final : public GameLibrary::Component
    {
    public:
        ActivePiece(GameLibrary::Actor* owner, const TetrisConfig& config, Board& board,
                    GameLibrary::IInputProvider& input);

        void Update(float deltaTime) override;
        void Render(GameLibrary::IGraphics& graphics) override;

        void Spawn(ETetromino type);

    private:
        void MoveLeft();
        void MoveRight();
        void MoveDown();
        void RotateCW();
        void RotateCCW();
        [[nodiscard]] bool CanMove(int32_t newX, int32_t newY, int32_t newRotation)  const noexcept;

        const TetrisConfig& m_config;
        Board& m_board;
        GameLibrary::IInputProvider& m_input;

        ETetromino m_type{ETetromino::I};
        int32_t m_gridX{};
        int32_t m_gridY{};
        int32_t m_rotation{};
        bool m_active{};
    };
} // namespace Tetris

#endif // ACTIVEPIECE_HPP
