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

        // 이벤트 구독
        m_pieceLockedToken = m_eventService->Subscribe<PieceLockedEvent>(
            [this](const PieceLockedEvent& event) { OnPieceLocked(event); });

        SpawnNewPiece();
    }

    void SingleGameScene::Update(const float deltaTime)
    {
        Scene::Update(deltaTime);

        // 홀드 입력
        if (m_input->IsKeyPressed(GameLibrary::KeyCode::C) ||
            m_input->IsKeyPressed(GameLibrary::KeyCode::LShift))
        {
            Hold();
        }
    }

    void SingleGameScene::SpawnNewPiece()
    {
        std::uniform_int_distribution<int32_t> dist(0, static_cast<int32_t>(ETetromino::Count) - 1);
        const auto type = static_cast<ETetromino>(dist(m_rng));
        m_activePiece->Spawn(type);
    }

    void SingleGameScene::Hold()
    {
        if (not m_activePiece->IsActive() || not m_holdBox->CanHold())
        {
            return;
        }

        const ETetromino currentType = m_activePiece->GetType();
        const auto swapped = m_holdBox->Hold(currentType);

        if (swapped.has_value())
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
        SpawnNewPiece();
    }
} // namespace Tetris