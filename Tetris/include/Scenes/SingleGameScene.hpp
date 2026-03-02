#pragma once

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

    private:
        void SpawnNewPiece();
        void Hold();
        void OnPieceLocked(const struct PieceLockedEvent& event);

        const TetrisConfig* m_tetrisConfig{};
        GameLibrary::IInputProvider* m_input{};
        GameLibrary::EventService* m_eventService{};

        Board* m_board{};
        ActivePiece* m_activePiece{};
        HoldBox* m_holdBox{};

        GameLibrary::SubscriptionToken m_pieceLockedToken{};

        std::mt19937 m_rng{std::random_device{}()};
    };
} // namespace Tetris