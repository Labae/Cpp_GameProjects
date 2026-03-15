#include "Scenes/SingleGameScene.hpp"

#include "Components/ActivePiece.hpp"
#include "Components/Board.hpp"
#include "Components/HoldBox.hpp"
#include "Components/NextQueue.hpp"
#include "Config/TetrisConfig.hpp"
#include "Core/EngineConfig.hpp"
#include "Core/ServiceContainer.hpp"
#include "Data/GameData.hpp"
#include "Events/GameEvents.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Scene/SceneManager.hpp"
#include "Scenes/States/GameOverState.hpp"
#include "Scenes/States/PausedState.hpp"
#include "Scenes/States/PlayingState.hpp"
#include "Services/EventService.hpp"
#include "Services/SaveService.hpp"
#include "Systems/Logger.hpp"

namespace
{
    constexpr sf::Color COLOR_HIGH_SCORE{180, 180, 180, 255};
    constexpr sf::Color COLOR_NEW_HIGH_SCORE{255, 215, 0, 255};

    constexpr auto SAVE_FILE_PATH = "tetris_save.dat";
    constexpr auto KEY_HIGH_SCORE = "highScore";
} // namespace

namespace Tetris
{
    SingleGameScene::SingleGameScene(const std::string& name, GameLibrary::ServiceContainer& container)
        : Scene(name, container)
    {
    }

    void SingleGameScene::OnEnter()
    {
        auto& container = GetContainer();
        m_engineConfig = container.Resolve<GameLibrary::EngineConfig>();
        m_tetrisConfig = container.Resolve<TetrisConfig>();
        m_input = container.Resolve<GameLibrary::IInputProvider>();
        m_eventService = container.Resolve<GameLibrary::EventService>();
        m_saveService = container.Resolve<GameLibrary::SaveService>();
        m_gameData = container.Resolve<GameData>();

        // InputActionMap
        m_actionMap.emplace(*m_input);
        m_actionMap->Map("MoveLeft", GameLibrary::KeyCode::Left);
        m_actionMap->Map("MoveRight", GameLibrary::KeyCode::Right);
        m_actionMap->Map("SoftDrop", GameLibrary::KeyCode::Down);
        m_actionMap->Map("HardDrop", GameLibrary::KeyCode::Space);
        m_actionMap->Map("RotateCW", {GameLibrary::KeyCode::Up, GameLibrary::KeyCode::X});
        m_actionMap->Map("RotateCCW", GameLibrary::KeyCode::Z);
        m_actionMap->Map("Hold", {GameLibrary::KeyCode::C, GameLibrary::KeyCode::LShift});
        m_actionMap->Map("Pause", GameLibrary::KeyCode::Escape);

        // Board
        auto* boardActor = CreateActor();
        m_board = boardActor->AddComponent<Board>(*m_tetrisConfig);

        // ActivePiece
        auto* pieceActor = CreateActor();
        m_activePiece = pieceActor->AddComponent<ActivePiece>(*m_tetrisConfig, *m_board, *m_actionMap, *m_eventService);

        // HoldBox
        auto* holdActor = CreateActor();
        m_holdBox = holdActor->AddComponent<HoldBox>(*m_tetrisConfig);

        // NextQueue
        auto* nextQueueActor = CreateActor();
        m_nextQueue = nextQueueActor->AddComponent<NextQueue>(*m_tetrisConfig, 5);

        // 이벤트 구독
        m_pieceLockedToken = m_eventService->Subscribe<PieceLockedEvent>([this](const PieceLockedEvent& event)
                                                                         { OnPieceLocked(event); });
        m_lineClearedToken = m_eventService->Subscribe<LinesClearedEvent>([this](const LinesClearedEvent& event)
                                                                          { OnLineCleared(event); });
        m_gameOverToken = m_eventService->Subscribe<GameOverEvent>([this](const GameOverEvent& event)
                                                                    { OnGameOver(event); });

        // 상태 등록 및 초기 상태 설정
        m_stateMachine.AddState<PlayingState>();
        m_stateMachine.AddState<PausedState>();
        m_stateMachine.AddState<GameOverState>();
        m_stateMachine.ChangeState<PlayingState>();

        SpawnNewPiece();
    }

    void SingleGameScene::Update(const float deltaTime)
    {
        m_stateMachine.Update(deltaTime);
    }

    void SingleGameScene::Render(GameLibrary::IGraphics& graphics)
    {
        Scene::Render(graphics);

        // 점수 표시
        graphics.DrawLabel("SCORE", 60, 400, 16, sf::Color::White, GameLibrary::TextAlign::Center);
        graphics.DrawLabel(std::to_string(m_score), 60, 425, 24, sf::Color::White, GameLibrary::TextAlign::Center);

        // 최고 점수 표시
        if (m_gameData)
        {
            graphics.DrawLabel("BEST", 60, 470, 16, COLOR_HIGH_SCORE, GameLibrary::TextAlign::Center);
            graphics.DrawLabel(std::to_string(m_gameData->highScore), 60, 495, 24, COLOR_HIGH_SCORE,
                               GameLibrary::TextAlign::Center);
        }

        const int32_t centerX = m_engineConfig->screenWidth / 2;
        const int32_t centerY = m_engineConfig->screenHeight / 2;

        // 상태별 오버레이 렌더링 (PausedState 등)
        m_stateMachine.Render(graphics);

        // 게임 오버 오버레이
        if (m_stateMachine.IsInState<GameOverState>())
        {
            graphics.FillRect(0, 0, m_engineConfig->screenWidth, m_engineConfig->screenHeight, sf::Color(0, 0, 0, 180));

            graphics.DrawLabel("GAME OVER", centerX, centerY - 80, 48, sf::Color::Red, GameLibrary::TextAlign::Center);
            graphics.DrawLabel("FINAL SCORE", centerX, centerY - 10, 20, sf::Color::White,
                               GameLibrary::TextAlign::Center);
            graphics.DrawLabel(std::to_string(m_score), centerX, centerY + 25, 36, sf::Color::Yellow,
                               GameLibrary::TextAlign::Center);

            if (m_isNewHighScore)
            {
                graphics.DrawLabel("NEW HIGH SCORE!", centerX, centerY + 80, 24, COLOR_NEW_HIGH_SCORE,
                                   GameLibrary::TextAlign::Center);
            }
            else if (m_gameData)
            {
                const std::string bestText = "BEST: " + std::to_string(m_gameData->highScore);
                graphics.DrawLabel(bestText, centerX, centerY + 80, 20, COLOR_HIGH_SCORE,
                                   GameLibrary::TextAlign::Center);
            }

            graphics.DrawLabel("Press ESC to return to title", centerX, centerY + 130, 16, sf::Color::Cyan,
                               GameLibrary::TextAlign::Center);
        }
    }

    void SingleGameScene::UpdateActors(const float deltaTime)
    {
        Scene::Update(deltaTime);
    }

    void SingleGameScene::Hold() const
    {
        if (not m_activePiece->IsActive() || not m_holdBox->CanHold())
        {
            return;
        }

        const ETetromino currentType = m_activePiece->GetType();
        if (const auto swapped = m_holdBox->Hold(currentType); swapped.has_value())
        {
            m_activePiece->Spawn(swapped.value());
        }
        else
        {
            SpawnNewPiece();
        }
    }

    void SingleGameScene::SpawnNewPiece() const
    {
        const ETetromino type = m_nextQueue->Pop();
        m_activePiece->Spawn(type);
    }

    void SingleGameScene::OnPieceLocked([[maybe_unused]] const PieceLockedEvent& event)
    {
        m_holdBox->ResetCanHold();

        const ETetromino nextType = m_nextQueue->Peek();
        if (not m_board->CanSpawnAt(nextType))
        {
            m_stateMachine.ChangeState<GameOverState>();
            m_eventService->Publish(GameOverEvent{});
            return;
        }

        SpawnNewPiece();
    }

    void SingleGameScene::OnGameOver([[maybe_unused]] const GameOverEvent& event)
    {
        if (not m_gameData)
        {
            return;
        }

        m_gameData->lastScore = m_score;

        if (m_score > m_gameData->highScore)
        {
            m_gameData->highScore = m_score;
            m_isNewHighScore = true;

            if (m_saveService)
            {
                m_saveService->SetInt(KEY_HIGH_SCORE, m_gameData->highScore);
                if (not m_saveService->SaveToFile(SAVE_FILE_PATH))
                {
                    GameLibrary::Logger::Error("Failed to save high score");
                }
            }
        }
    }

    void SingleGameScene::OnLineCleared(const LinesClearedEvent& event)
    {
        if (event.lineCount > 0 && event.lineCount <= static_cast<int32_t>(m_tetrisConfig->lineScores.size()))
        {
            m_score += m_tetrisConfig->lineScores[event.lineCount - 1];
        }
    }
} // namespace Tetris