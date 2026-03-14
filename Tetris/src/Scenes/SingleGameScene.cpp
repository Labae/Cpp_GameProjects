#include "Scenes/SingleGameScene.hpp"

#include "Components/ActivePiece.hpp"
#include "Components/Board.hpp"
#include "Components/HoldBox.hpp"
#include "Config/TetrisConfig.hpp"
#include "Core/ServiceContainer.hpp"
#include "Events/GameEvents.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Services/EventService.hpp"

namespace Tetris
{
    SingleGameScene::SingleGameScene(const std::string& name, GameLibrary::ServiceContainer& container)
        : Scene(name, container)
    {
    }

    void SingleGameScene::OnEnter()
    {
        auto& container = GetContainer();
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

        SpawnNewPiece();
    }

    void SingleGameScene::Update(const float deltaTime)
    {
        if (m_isGameOver)
        {
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

        if (m_isGameOver)
        {
            graphics.DrawLabel("GAME_OVER", 400, 400, 48, sf::Color::Red, GameLibrary::TextAlign::Center);
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
} // namespace Tetris