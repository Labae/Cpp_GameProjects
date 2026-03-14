#pragma once

#include "Components/NextQueue.hpp"
#include "Data/Tetromino.hpp"
#include "Scene/Scene.hpp"
#include "Services/EventService.hpp"

#include <random>

namespace GameLibrary
{
    class IInputProvider;
}

namespace Tetris
{
    struct TetrisConfig;
    class Board;
    class ActivePiece;
    class HoldBox;

    class SingleGameScene final : public GameLibrary::Scene
    {
    public:
        SingleGameScene(const std::string& name, GameLibrary::ServiceContainer& container);

        void OnEnter() override;
        void Update(float deltaTime) override;
        void Render(GameLibrary::IGraphics& graphics) override;

    private:
        void SpawnNewPiece() const;
        void Hold() const;
        void OnPieceLocked(const struct PieceLockedEvent& event);

        const TetrisConfig* m_tetrisConfig{};
        GameLibrary::IInputProvider* m_input{};
        GameLibrary::EventService* m_eventService{};

        Board* m_board{};
        ActivePiece* m_activePiece{};
        HoldBox* m_holdBox{};
        NextQueue* m_nextQueue{};

        GameLibrary::SubscriptionToken m_pieceLockedToken{};

        bool m_isGameOver{};
    };
} // namespace Tetris