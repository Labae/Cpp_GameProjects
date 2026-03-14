#include "Scenes/SingleGameScene.hpp"

#include "Components/ActivePiece.hpp"
#include "Components/Board.hpp"
#include "Components/HoldBox.hpp"
#include "Config/TetrisConfig.hpp"
#include "Core/ServiceContainer.hpp"
#include "Events/GameEvents.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Scene/SceneManager.hpp"
#include "Services/EventService.hpp"

namespace
{
    constexpr sf::Color MENU_SELECTED_COLOR = sf::Color::Yellow;
    constexpr auto MENU_NORMAL_COLOR = sf::Color(150, 150, 150, 255);
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

        // Board
        auto* boardActor = CreateActor();
        m_board = boardActor->AddComponent<Board>(*m_tetrisConfig);

        // ActivePiece
        auto* pieceActor = CreateActor();
        m_activePiece = pieceActor->AddComponent<ActivePiece>(*m_tetrisConfig, *m_board, *m_input, *m_eventService);

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

        SpawnNewPiece();
    }

    void SingleGameScene::Update(const float deltaTime)
    {
        // 게임 종료
        if (m_isGameOver)
        {
            if (m_input->IsKeyPressed(GameLibrary::KeyCode::Escape))
            {
                GetContainer().Resolve<GameLibrary::SceneManager>()->LoadScene("Title");
            }
            return;
        }

        // 일시 정지
        if (m_input->IsKeyPressed(GameLibrary::KeyCode::Escape))
        {
            m_isPaused = not m_isPaused;
            m_pauseMenuIndex = 0;
            return;
        }

        if (m_isPaused)
        {
            if (m_input->IsKeyPressed(GameLibrary::KeyCode::Up))
            {
                m_pauseMenuIndex = (m_pauseMenuIndex + 1) % 2;
            }
            if (m_input->IsKeyPressed(GameLibrary::KeyCode::Down))
            {
                m_pauseMenuIndex = (m_pauseMenuIndex + 1) % 2;
            }
            if (m_input->IsKeyPressed(GameLibrary::KeyCode::Enter))
            {
                if (m_pauseMenuIndex == 0)
                {
                    m_isPaused = false; // Resume
                }
                else
                {
                    GetContainer().Resolve<GameLibrary::SceneManager>()->LoadScene("Title");
                }
            }
            return;
        }

        Scene::Update(deltaTime);

        // 홀드 입력
        if (m_input->IsKeyPressed(GameLibrary::KeyCode::C) || m_input->IsKeyPressed(GameLibrary::KeyCode::LShift))
        {
            Hold();
        }
    }

    void SingleGameScene::Render(GameLibrary::IGraphics& graphics)
    {
        Scene::Render(graphics);

        // 점수 표시
        graphics.DrawLabel("SCORE", 60, 400, 16, sf::Color::White, GameLibrary::TextAlign::Center);
        graphics.DrawLabel(std::to_string(m_score), 60, 425, 24, sf::Color::White, GameLibrary::TextAlign::Center);

        const int32_t centerX = m_engineConfig->screenWidth / 2;
        const int32_t centerY = m_engineConfig->screenHeight / 2;

        // 일시정지 메뉴
        if (m_isPaused)
        {
            // 오버레이
            graphics.FillRect(0, 0, m_engineConfig->screenWidth, m_engineConfig->screenHeight, sf::Color(0, 0, 0, 180));

            // 일시정지 텍스트
            graphics.DrawLabel("PAUSED", centerX, centerY - 80, 48, sf::Color::White, GameLibrary::TextAlign::Center);

            graphics.DrawLabel("Resume", centerX, centerY, 24,
                               m_pauseMenuIndex == 0 ? MENU_SELECTED_COLOR : MENU_NORMAL_COLOR,
                               GameLibrary::TextAlign::Center);
            graphics.DrawLabel("Title", centerX, centerY + 40, 24,
                               m_pauseMenuIndex == 1 ? MENU_SELECTED_COLOR : MENU_NORMAL_COLOR,
                               GameLibrary::TextAlign::Center);

            return;
        }

        // 게임 오버
        if (m_isGameOver)
        {
            // 오버레이
            graphics.FillRect(0, 0, m_engineConfig->screenWidth, m_engineConfig->screenHeight, sf::Color(0, 0, 0, 180));

            // 게임오버 텍스트
            graphics.DrawLabel("GAME OVER", centerX, centerY - 50, 48, sf::Color::Red, GameLibrary::TextAlign::Center);

            // 최종 점수
            graphics.DrawLabel("FINAL SCORE", centerX, centerY + 20, 20, sf::Color::White,
                               GameLibrary::TextAlign::Center);
            graphics.DrawLabel(std::to_string(m_score), centerX, centerY + 55, 36, sf::Color::Yellow,
                               GameLibrary::TextAlign::Center);

            // 안내
            graphics.DrawLabel("Press ESC to return to title", centerX, centerY + 120, 16, sf::Color::Cyan,
                               GameLibrary::TextAlign::Center);
        }
    }

    void SingleGameScene::SpawnNewPiece() const
    {
        const ETetromino type = m_nextQueue->Pop();
        m_activePiece->Spawn(type);
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

    void SingleGameScene::OnPieceLocked([[maybe_unused]] const PieceLockedEvent& event)
    {
        m_holdBox->ResetCanHold();

        const ETetromino nextType = m_nextQueue->Peek();
        if (not m_board->CanSpawnAt(nextType))
        {
            m_isGameOver = true;
            m_eventService->Publish(GameOverEvent{});
            return;
        }

        SpawnNewPiece();
    }

    void SingleGameScene::OnLineCleared(const LinesClearedEvent& event)
    {
        if (event.lineCount > 0 && event.lineCount <= static_cast<int32_t>(m_tetrisConfig->lineScores.size()))
        {
            m_score += m_tetrisConfig->lineScores[event.lineCount - 1];
        }
    }
} // namespace Tetris