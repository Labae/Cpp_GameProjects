#pragma once

#include "Components/Component.hpp"
#include "Data/Tetromino.hpp"
#include "Time/Timer.hpp"

#include <cstdint>

namespace GameLibrary
{
    class InputActionMap;
    class EventService;
}

namespace Tetris
{
    struct TetrisConfig;
    class Board;

    class ActivePiece final : public GameLibrary::Component
    {
    public:
        ActivePiece(GameLibrary::Actor* owner, const TetrisConfig& config, Board& board,
                    GameLibrary::InputActionMap& actionMap, GameLibrary::EventService& eventService);

        void Update(float deltaTime) override;
        void Render(GameLibrary::IGraphics& graphics) override;

        void Spawn(ETetromino type);
        [[nodiscard]] bool IsActive() const noexcept { return m_active; }
        [[nodiscard]] ETetromino GetType() const noexcept { return m_type; }

    private:
        void HandleInput();
        void MoveLeft();
        void MoveRight();
        void SoftDrop();
        void HardDrop();
        void RotateCW();
        void RotateCCW();
        void Lock();

        [[nodiscard]] bool CanMove(int32_t newX, int32_t newY, int32_t newRotation) const;
        [[nodiscard]] int32_t CalculateGhostY() const;

        const TetrisConfig& m_config;
        Board& m_board;
        GameLibrary::InputActionMap& m_actionMap;
        GameLibrary::EventService& m_eventService;

        ETetromino m_type{ETetromino::I};
        int32_t m_gridX{};
        int32_t m_gridY{};
        int32_t m_rotation{};
        bool m_active{};

        GameLibrary::Timer m_fallTimer;
    };
} // namespace Tetris