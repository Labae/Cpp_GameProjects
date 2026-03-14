#pragma once

#include "Events/GameEvents.hpp"
#include "Input/InputActionMap.hpp"
#include "Scene/Scene.hpp"
#include "Services/EventService.hpp"
#include "States/StateMachine.hpp"

#include <cstdint>
#include <optional>

namespace GameLibrary
{
    class IInputProvider;
    class SaveService;
    struct EngineConfig;
} // namespace GameLibrary

namespace Tetris
{
    struct TetrisConfig;
    struct GameData;
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
        [[nodiscard]] GameLibrary::InputActionMap& GetActionMap() noexcept { return *m_actionMap; }
        [[nodiscard]] GameLibrary::StateMachine<SingleGameScene>& GetStateMachine() noexcept { return m_stateMachine; }
        [[nodiscard]] const GameLibrary::EngineConfig& GetEngineConfig() const noexcept { return *m_engineConfig; }
        [[nodiscard]] int32_t GetScore() const noexcept { return m_score; }

        void UpdateActors(float deltaTime);
        void Hold() const;

    private:
        void SpawnNewPiece() const;
        void OnPieceLocked(const PieceLockedEvent& event);
        void OnLineCleared(const LinesClearedEvent& event);
        void OnGameOver(const GameOverEvent& event);

        GameLibrary::StateMachine<SingleGameScene> m_stateMachine{*this};

        GameLibrary::EngineConfig* m_engineConfig{};
        TetrisConfig* m_tetrisConfig{};
        GameLibrary::IInputProvider* m_input{};
        GameLibrary::EventService* m_eventService{};
        GameLibrary::SaveService* m_saveService{};
        GameData* m_gameData{};
        std::optional<GameLibrary::InputActionMap> m_actionMap{};

        Board* m_board{};
        ActivePiece* m_activePiece{};
        HoldBox* m_holdBox{};
        NextQueue* m_nextQueue{};

        int32_t m_score{};
        bool m_isNewHighScore{};

        GameLibrary::SubscriptionToken m_pieceLockedToken{};
        GameLibrary::SubscriptionToken m_lineClearedToken{};
        GameLibrary::SubscriptionToken m_gameOverToken{};
    };
} // namespace Tetris