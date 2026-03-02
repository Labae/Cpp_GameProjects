//
// Created by Labae on 3/1/26.
//

#include "Scenes/SingleGameScene.hpp"

#include "Components/ActivePiece.hpp"
#include "Components/Board.hpp"
#include "Core/ServiceContainer.hpp"
#include "Data/Tetromino.hpp"
#include "Interfaces/IInputProvider.hpp"

namespace
{
    constexpr int32_t HOLD_BOX_X = 60;
    constexpr int32_t HOLD_BOX_Y = 100;
    constexpr int32_t HOLD_BOX_PADDING = 10;
    constexpr sf::Color COLOR_BOX_BG{30, 30, 30, 255};
    constexpr sf::Color COLOR_BOX_BORDER{100, 100, 100, 255};
    constexpr sf::Color COLOR_HOLD_DISABLED{100, 100, 100, 128};
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
        m_tetrisConfig = container.Resolve<TetrisConfig>();
        m_inputProvider = container.Resolve<GameLibrary::IInputProvider>();

        // Board
        auto* boardActor = CreateActor();
        m_board = boardActor->AddComponent<Board>(*m_tetrisConfig);

        // Piece
        auto* pieceActor = CreateActor();
        m_activePiece = pieceActor->AddComponent<ActivePiece>(*m_tetrisConfig, *m_board, *m_inputProvider);
        SpawnNewPiece();
    }

    void SingleGameScene::Update(const float deltaTime)
    {
        Scene::Update(deltaTime);

        if (m_inputProvider->IsKeyPressed(GameLibrary::KeyCode::C) or
            m_inputProvider->IsKeyPressed(GameLibrary::KeyCode::LShift))
        {
            Hold();
        }

        if (not m_activePiece->IsActive())
        {
            m_canHold = true;
            SpawnNewPiece();
        }
    }
    void SingleGameScene::Render(GameLibrary::IGraphics& graphics)
    {
        Scene::Render(graphics);
        RenderHoldBox(graphics);
    }

    void SingleGameScene::SpawnNewPiece()
    {
        std::uniform_int_distribution<int32_t> dist(0, static_cast<int32_t>(ETetromino::Count) - 1);
        const auto type = static_cast<ETetromino>(dist(m_rnd));
        m_activePiece->Spawn(type);
    }

    void SingleGameScene::Hold()
    {
        if (not m_canHold or not m_activePiece->IsActive())
        {
            return;
        }

        const ETetromino currentType = m_activePiece->GetType();
        if (m_holdPiece.has_value())
        {
            // 홀드된 피스와 교체
            const ETetromino holdType = m_holdPiece.value();
            m_holdPiece = currentType;
            m_activePiece->Spawn(holdType);
        }
        else
        {
            // 첫 홀드
            m_holdPiece = currentType;
            SpawnNewPiece();
        }

        m_canHold = false;
    }

    void SingleGameScene::RenderHoldBox(GameLibrary::IGraphics& graphics) const
    {
        const int32_t boxSize = m_tetrisConfig->holdNextBoxSize;

        graphics.FillRect(HOLD_BOX_X, HOLD_BOX_Y, boxSize, boxSize, COLOR_BOX_BG);
        graphics.DrawRect(HOLD_BOX_X, HOLD_BOX_Y, boxSize, boxSize, COLOR_BOX_BORDER);

        graphics.DrawLabel("HOLD", HOLD_BOX_X + boxSize / 2, HOLD_BOX_Y - 25, 16, sf::Color::White,
                           GameLibrary::TextAlign::Center);

        if (not m_holdPiece.has_value())
        {
            return;
        }

        const auto& tetromino = GetTetromino(m_holdPiece.value());
        const sf::Color color = m_canHold ? tetromino.color : COLOR_HOLD_DISABLED;
        const int32_t cellSize = boxSize / 5;
        constexpr int32_t offsetX = HOLD_BOX_X + HOLD_BOX_PADDING;
        constexpr int32_t offsetY = HOLD_BOX_Y + HOLD_BOX_PADDING;

        for (int32_t row = 0; row < 4; ++row)
        {
            for (int32_t col = 0; col < 4; ++col)
            {
                if (tetromino.shapes[0][row][col] == 1)
                {
                    const int32_t px = offsetX + col * cellSize;
                    const int32_t py = offsetY + row * cellSize;
                    graphics.FillRect(px, py, cellSize, cellSize, color);
                }
            }
        }
    }
} // namespace Tetris