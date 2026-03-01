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
        auto* activePiece = pieceActor->AddComponent<ActivePiece>(*m_tetrisConfig, *board, *input);
        activePiece->Spawn(ETetromino::I);
    }
} // namespace Tetris