//
// Created by Labae on 3/1/26.
//

#include "Components/ActivePiece.hpp"

#include "Components/Board.hpp"
#include "Configs/TetrisConfig.hpp"
#include "Interfaces/IGraphics.hpp"

namespace Tetris
{
    ActivePiece::ActivePiece(GameLibrary::Actor* owner, const TetrisConfig& config, Board& board,
                             GameLibrary::IInputProvider& input)
        : Component(owner)
        , m_config(config)
        , m_board(board)
        , m_input(input)
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

    void ActivePiece::Render(GameLibrary::IGraphics& graphics)
    {
        if (not m_active)
        {
            return;
        }

        const auto& tetromino = GetTetromino(m_type);

        for (int32_t row = 0; row < 4; ++row)
        {
            for (int32_t col = 0; col < 4; ++col)
            {
                if (tetromino.shapes[m_rotation][row][col] != 1)
                {
                    continue;
                }

                const int32_t px = m_config.boardX + (m_gridX + col) * m_config.cellSize;
                const int32_t py = m_config.boardY + (m_gridY + row) * m_config.cellSize;
                graphics.FillRect(px, py, m_config.cellSize, m_config.cellSize, tetromino.color);
            }
        }
    }

    void ActivePiece::Spawn(const ETetromino type)
    {
        m_type = type;
        m_gridX = (m_config.boardWidth - 4) / 2; // center
        m_gridY = 0;
        m_rotation = 0;
        m_active = true;
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
        if (m_input.IsKeyPressed(GameLibrary::KeyCode::Up) || m_input.IsKeyPressed(GameLibrary::KeyCode::X))
        {
            RotateCW();
        }
        if (m_input.IsKeyPressed(GameLibrary::KeyCode::Z))
        {
            RotateCCW();
        }
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
        if (const int32_t newRotation = (m_rotation + 1) % 4; CanMove(m_gridX, m_gridY, newRotation))
        {
            m_rotation = newRotation;
        }
    }

    void ActivePiece::RotateCCW()
    {
        if (const int32_t newRotation = (m_rotation + 3) % 4; CanMove(m_gridX, m_gridY, newRotation))
        {
            m_rotation = newRotation;
        }
    }

    void ActivePiece::Lock()
    {
        const auto& tetromino = GetTetromino(m_type);
        for (int32_t row = 0; row < 4; ++row)
        {
            for (int32_t col = 0; col < 4; ++col)
            {
                if (tetromino.shapes[m_rotation][row][col] != 1)
                {
                    continue;
                }

                const int32_t boardX = m_gridX + col;
                const int32_t boardY = m_gridY + row;
                m_board.SetCell(boardX, boardY, static_cast<uint8_t>(m_type) + 1);
            }
        }

        m_board.ClearFullLines();
        m_active = false;
    }

    bool ActivePiece::CanMove(const int32_t newX, const int32_t newY, const int32_t newRotation) const noexcept
    {
        const auto& tetromino = GetTetromino(m_type);
        for (int32_t row = 0; row < 4; ++row)
        {
            for (int32_t col = 0; col < 4; ++col)
            {
                if (tetromino.shapes[newRotation][row][col] != 1)
                {
                    continue;
                }

                const int32_t boardX = newX + col;
                const int32_t boardY = newY + row;

                if (boardX < 0 || boardX >= m_config.boardWidth)
                {
                    return false;
                }

                if (boardY < 0 || boardY >= m_config.boardHeight)
                {
                    return false;
                }

                if (not m_board.IsCellEmpty(boardX, boardY))
                {
                    return false;
                }
            }
        }

        return true;
    }
} // namespace Tetris