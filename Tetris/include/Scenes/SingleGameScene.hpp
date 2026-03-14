#pragma once

#include "Scene/Scene.hpp"
#include "Services/EventService.hpp"
#include "States/StateMachine.hpp"

#include <cstdint>

namespace GameLibrary
{
    class IInputProvider;
    struct EngineConfig;
} // namespace GameLibrary

namespace Tetris
{
    struct TetrisConfig;
    class Board;
    class ActivePiece;
    class HoldBox;
    class NextQueue;
    struct PieceLockedEvent;
    struct LinesClearedEvent;

    class SingleGameScene final : public GameLibrary::Scene
    {
    public:
        SingleGameScene(const std::string& name, GameLibrary::ServiceContainer& container);

        void OnEnter() override;
        void Update(float deltaTime) override;
        void Render(GameLibrary::IGraphics& graphics) override;

        // State에서 접근하는 public 인터페이스
        [[nodiscard]] GameLibrary::IInputProvider& GetInput() const noexcept { return *m_input; }
        [[nodiscard]] GameLibrary::StateMachine<SingleGameScene>& GetStateMachine() noexcept { return m_stateMachine; }
        [[nodiscard]] const GameLibrary::EngineConfig& GetEngineConfig() const noexcept { return *m_engineConfig; }
        [[nodiscard]] int32_t GetScore() const noexcept { return m_score; }

        void UpdateActors(float deltaTime);
        void Hold() const;

    private:
        void SpawnNewPiece() const;
        void OnPieceLocked(const PieceLockedEvent& event);
        void OnLineCleared(const LinesClearedEvent& event);

        GameLibrary::StateMachine<SingleGameScene> m_stateMachine{*this};

        GameLibrary::EngineConfig* m_engineConfig{};
        TetrisConfig* m_tetrisConfig{};
        GameLibrary::IInputProvider* m_input{};
        GameLibrary::EventService* m_eventService{};

        Board* m_board{};
        ActivePiece* m_activePiece{};
        HoldBox* m_holdBox{};
        NextQueue* m_nextQueue{};

        int32_t m_score{};

        GameLibrary::SubscriptionToken m_pieceLockedToken{};
        GameLibrary::SubscriptionToken m_lineClearedToken{};
    };
} // namespace Tetris