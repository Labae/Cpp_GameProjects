#include "Components/ActivePiece.hpp"

#include "Components/Board.hpp"
#include "Config/TetrisConfig.hpp"
#include "Data/Cell.hpp"
#include "Events/GameEvents.hpp"
#include "Interfaces/IGraphics.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Services/EventService.hpp"

namespace Tetris
{
    ActivePiece::ActivePiece(GameLibrary::Actor* owner, const TetrisConfig& config, Board& board,
                             GameLibrary::IInputProvider& input, GameLibrary::EventService& eventService)
        : Component(owner)
        , m_config(config)
        , m_board(board)
        , m_input(input)
        , m_eventService(eventService)
        , m_fallInterval(config.fallInterval)
    {
    }

    void ActivePiece::Update(const float deltaTime)
    {
        if (not m_active)
        {
            return;
        }

        HandleInput();

        m_fallTimer += deltaTime;
        if (m_fallTimer >= m_fallInterval)
        {
            m_fallTimer = 0.0f;

            if (CanMove(m_gridX, m_gridY + 1, m_rotation))
            {
                ++m_gridY;
            }
            else
            {
                Lock();
            }
        }
    }

    void ActivePiece::HandleInput()
    {
        if (m_input.IsKeyPressed(GameLibrary::KeyCode::Left))
        {
            MoveLeft();
        }
        if (m_input.IsKeyPressed(GameLibrary::KeyCode::Right))
        {
            MoveRight();
        }
        if (m_input.IsKeyPressed(GameLibrary::KeyCode::Down))
        {
            SoftDrop();
        }
        if (m_input.IsKeyPressed(GameLibrary::KeyCode::Space))
        {
            HardDrop();
        }
        if (m_input.IsKeyPressed(GameLibrary::KeyCode::Up) ||
            m_input.IsKeyPressed(GameLibrary::KeyCode::X))
        {
            RotateCW();
        }
        if (m_input.IsKeyPressed(GameLibrary::KeyCode::Z))
        {
            RotateCCW();
        }
    }

    void ActivePiece::Render(GameLibrary::IGraphics& graphics)
    {
        if (not m_active)
        {
            return;
        }

        const auto& tetromino = GetTetromino(m_type);
        const Cell cell(m_type);

        for (const auto& [col, row] : tetromino.GetBlockPositions(m_rotation))
        {
            const int32_t px = m_config.boardX + (m_gridX + col) * m_config.cellSize;
            const int32_t py = m_config.boardY + (m_gridY + row) * m_config.cellSize;
            cell.Render(graphics, px, py, m_config.cellSize);
        }
    }

    void ActivePiece::Spawn(const ETetromino type)
    {
        m_type = type;
        m_gridX = (m_config.boardWidth - 4) / 2;
        m_gridY = 0;
        m_rotation = 0;
        m_fallTimer = 0.0f;
        m_active = true;
    }

    void ActivePiece::MoveLeft()
    {
        if (CanMove(m_gridX - 1, m_gridY, m_rotation))
        {
            --m_gridX;
        }
    }

    void ActivePiece::MoveRight()
    {
        if (CanMove(m_gridX + 1, m_gridY, m_rotation))
        {
            ++m_gridX;
        }
    }

    void ActivePiece::SoftDrop()
    {
        if (CanMove(m_gridX, m_gridY + 1, m_rotation))
        {
            ++m_gridY;
            m_fallTimer = 0.0f;
        }
    }

    void ActivePiece::HardDrop()
    {
        while (CanMove(m_gridX, m_gridY + 1, m_rotation))
        {
            ++m_gridY;
        }
        Lock();
    }

    void ActivePiece::RotateCW()
    {
        const int32_t newRotation = (m_rotation + 1) % 4;
        if (CanMove(m_gridX, m_gridY, newRotation))
        {
            m_rotation = newRotation;
        }
    }

    void ActivePiece::RotateCCW()
    {
        const int32_t newRotation = (m_rotation + 3) % 4;
        if (CanMove(m_gridX, m_gridY, newRotation))
        {
            m_rotation = newRotation;
        }
    }

    void ActivePiece::Lock()
    {
        const auto& tetromino = GetTetromino(m_type);

        for (const auto& [col, row] : tetromino.GetBlockPositions(m_rotation))
        {
            m_board.SetCell(m_gridX + col, m_gridY + row, Cell(m_type));
        }

        m_board.ClearFullLines();
        m_active = false;

        m_eventService.Publish(PieceLockedEvent{});
    }

    bool ActivePiece::CanMove(const int32_t newX, const int32_t newY, const int32_t newRotation) const
    {
        const auto& tetromino = GetTetromino(m_type);

        for (const auto& [col, row] : tetromino.GetBlockPositions(newRotation))
        {
            const int32_t boardX = newX + col;
            const int32_t boardY = newY + row;

            if (boardX < 0 || boardX >= m_config.boardWidth ||
                boardY < 0 || boardY >= m_config.boardHeight ||
                not m_board.IsCellEmpty(boardX, boardY))
            {
                return false;
            }
        }

        return true;
    }
} // namespace Tetris