#include "SnakeController.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Interfaces/IGraphics.hpp"
#include "Systems/EventSystem.hpp"
#include "Core/EngineConfig.hpp"
#include "Config/SnakeGameConfig.hpp"
#include "Events/GameEvents.hpp"
#include <algorithm>

SnakeController::SnakeController(GameLibrary::Transform& transform,
    GameLibrary::IInputProvider& input,
    GameLibrary::EventSystem& eventSystem,
    const GameLibrary::EngineConfig& engineConfig,
    const SnakeGame::SnakeGameConfig& gameConfig)
    : m_transform(transform)
    , m_input(input)
    , m_eventSystem(eventSystem)
    , m_engineConfig(engineConfig)
    , m_gameConfig(gameConfig)
{
}

void SnakeController::Init()
{
    m_foodEatenToken = m_eventSystem.Subscribe<FoodEatenEvent>(
        [this](const FoodEatenEvent& event) { OnFoodEaten(event); }
    );

    m_gameOverToken = m_eventSystem.Subscribe<GameOverEvent>(
        [this](const GameOverEvent& event) { m_isGameOver = true; }
    );
}

void SnakeController::Update(float deltaTime)
{
    if (m_isGameOver)
    {
        return;
    }

    if (m_input.IsKeyPressed(GameLibrary::KeyCode::Up) && m_direction.y == 0)
    {
        m_direction = { 0.0f, -1.0f };
    }
    else if (m_input.IsKeyPressed(GameLibrary::KeyCode::Down) && m_direction.y == 0)
    {
        m_direction = { 0.0f, 1.0f };
    }
    else if (m_input.IsKeyPressed(GameLibrary::KeyCode::Left) && m_direction.x == 0)
    {
        m_direction = { -1.0f, 0.0f };
    }
    else if (m_input.IsKeyPressed(GameLibrary::KeyCode::Right) && m_direction.x == 0)
    {
        m_direction = { 1.0f, 0.0f };
    }

    m_moveTimer += deltaTime;
    if (m_moveTimer >= m_gameConfig.moveInterval)
    {
        m_moveTimer = 0.0f;

        m_body.insert(m_body.begin(), m_transform.position);

        m_transform.position.x += m_direction.x * m_gameConfig.gridSize;
        m_transform.position.y += m_direction.y * m_gameConfig.gridSize;

        if (CheckCollision())
        {
            m_eventSystem.Publish(GameOverEvent{ m_score });
            return;
        }

        if (m_shouldGrow)
        {
            m_shouldGrow = false;
        }
        else if (!m_body.empty())
        {
            m_body.pop_back();
        }

        m_eventSystem.Publish(SnakeMovedEvent{ m_transform.position });
    }
}

void SnakeController::Render(GameLibrary::IGraphics& graphics)
{
    auto totalSegmentCount = static_cast<int32_t>(m_body.size()) + 1;

    // index 0 머리 그리기
    {
        graphics.FillRect(
            static_cast<int>(m_transform.position.x),
            static_cast<int>(m_transform.position.y),
            m_gameConfig.gridSize,
            m_gameConfig.gridSize,
            { m_gameConfig.headColor.r, m_gameConfig.headColor.g, m_gameConfig.headColor.b, 255 }
        );
    }

    for (size_t i = 0; i < m_body.size(); ++i)
    {
        float t = static_cast<float>(i + 1) / std::max(1, totalSegmentCount - 1);

        GameLibrary::Color color
        {
            .r = static_cast<uint8_t>(m_gameConfig.headColor.r + (m_gameConfig.tailColor.r - m_gameConfig.headColor.r) * t),
            .g = static_cast<uint8_t>(m_gameConfig.headColor.g + (m_gameConfig.tailColor.g - m_gameConfig.headColor.g) * t),
            .b = static_cast<uint8_t>(m_gameConfig.headColor.b + (m_gameConfig.tailColor.b - m_gameConfig.headColor.b) * t),
            .a = 255
        };

        auto& segment = m_body[i];
        graphics.FillRect(
            static_cast<int>(segment.x),
            static_cast<int>(segment.y),
            m_gameConfig.gridSize,
            m_gameConfig.gridSize,
            color
        );
    }
}

bool SnakeController::CheckCollision() const
{
    if (m_transform.position.x < 0 || m_transform.position.x >= m_engineConfig.screenWidth ||
        m_transform.position.y < 0 || m_transform.position.y >= m_engineConfig.screenHeight)
    {
        return true;
    }

    for (const auto& segment : m_body)
    {
        if (m_transform.position == segment)
        {
            return true;
        }
    }

    return false;
}

void SnakeController::OnFoodEaten(const FoodEatenEvent& event)
{
    m_shouldGrow = true;
    m_score++;
    m_eventSystem.Publish(ScoreChangedEvent{ m_score });
}