//
// Created by Labae on 3/1/26.
//

#include "Scenes/SingleGameScene.hpp"

#include "Components/ActivePiece.hpp"
#include "Components/Board.hpp"
#include "Core/ServiceContainer.hpp"
#include "Data/Tetromino.hpp"
#include "Interfaces/IInputProvider.hpp"

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
        auto* input = container.Resolve<GameLibrary::IInputProvider>();

        // Board
        auto* boardActor = CreateActor();
        auto* board = boardActor->AddComponent<Board>(*m_tetrisConfig);

        // Piece
        auto* pieceActor = CreateActor();
        m_activePiece = pieceActor->AddComponent<ActivePiece>(*m_tetrisConfig, *board, *input);
        SpawnNewPiece();
    }
    
    void SingleGameScene::Update(const float deltaTime)
    {
        Scene::Update(deltaTime);

        if (not m_activePiece->IsActive())
        {
            SpawnNewPiece();
        }
    }

    void SingleGameScene::SpawnNewPiece()
    {
        std::uniform_int_distribution<int32_t> dist(0, static_cast<int32_t>(ETetromino::Count) - 1);
        const auto type = static_cast<ETetromino>(dist(m_rnd));
        m_activePiece->Spawn(type);
    }
} // namespace Tetris