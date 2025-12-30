#include "Components/SnakeController.hpp"

#include "Config/SnakeGameConfig.hpp"
#include "Core/EngineConfig.hpp"
#include "Events/GameEvents.hpp"
#include "Interfaces/IGraphics.hpp"
#include "Interfaces/IInputProvider.hpp"

#include <algorithm>

SnakeController::SnakeController(GameLibrary::Transform& transform, GameLibrary::IInputProvider& input,
                                 GameLibrary::EventSystem& eventSystem, const GameLibrary::EngineConfig& engineConfig,
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
    m_foodEatenToken =
        m_eventSystem.Subscribe<FoodEatenEvent>([this](const FoodEatenEvent& event) { OnFoodEaten(event); });

    m_gameOverToken = m_eventSystem.Subscribe<GameOverEvent>([this]([[maybe_unused]] const GameOverEvent& event)
                                                             { m_isGameOver = true; });
}

void SnakeController::Update(float deltaTime)
{
    if (m_isGameOver)
    {
        return;
    }

    if (m_input.IsKeyPressed(GameLibrary::KeyCode::Up) && m_direction.y == 0)
    {
        m_direction = {0.0f, -1.0f};
    }
    else if (m_input.IsKeyPressed(GameLibrary::KeyCode::Down) && m_direction.y == 0)
    {
        m_direction = {0.0f, 1.0f};
    }
    else if (m_input.IsKeyPressed(GameLibrary::KeyCode::Left) && m_direction.x == 0)
    {
        m_direction = {-1.0f, 0.0f};
    }
    else if (m_input.IsKeyPressed(GameLibrary::KeyCode::Right) && m_direction.x == 0)
    {
        m_direction = {1.0f, 0.0f};
    }

    m_moveTimer += deltaTime;
    if (m_moveTimer >= m_gameConfig.moveInterval)
    {
        m_moveTimer = 0.0f;

        m_body.insert(m_body.begin(), m_transform.position);

        m_transform.position.x += m_direction.x * static_cast<float>(m_gameConfig.gridSize);
        m_transform.position.y += m_direction.y * static_cast<float>(m_gameConfig.gridSize);

        if (CheckCollision())
        {
            m_eventSystem.Publish(GameOverEvent{m_score});
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

        m_eventSystem.Publish(SnakeMovedEvent{m_transform.position});
    }
}

void SnakeController::Render(GameLibrary::IGraphics& graphics)
{
    const auto totalSegmentCount = static_cast<int32_t>(m_body.size()) + 1;

    // 머리 그리기
    graphics.FillRect(static_cast<int32_t>(m_transform.position.x), static_cast<int32_t>(m_transform.position.y),
                      m_gameConfig.gridSize, m_gameConfig.gridSize, m_gameConfig.headColor);

    // 몸통 그리기 (그라데이션)
    for (size_t i = 0; i < m_body.size(); ++i)
    {
        const float t = static_cast<float>(i + 1) / static_cast<float>(std::max(1, totalSegmentCount - 1));

        const sf::Color color{
            static_cast<uint8_t>(
                static_cast<float>(m_gameConfig.headColor.r) +
                (static_cast<float>(m_gameConfig.tailColor.r) - static_cast<float>(m_gameConfig.headColor.r)) * t),
            static_cast<uint8_t>(
                static_cast<float>(m_gameConfig.headColor.g) +
                (static_cast<float>(m_gameConfig.tailColor.g) - static_cast<float>(m_gameConfig.headColor.g)) * t),
            static_cast<uint8_t>(
                static_cast<float>(m_gameConfig.headColor.b) +
                (static_cast<float>(m_gameConfig.tailColor.b) - static_cast<float>(m_gameConfig.headColor.b)) * t),
            255,
        };

        const auto& segment = m_body[i];
        graphics.FillRect(static_cast<int32_t>(segment.x), static_cast<int32_t>(segment.y), m_gameConfig.gridSize,
                          m_gameConfig.gridSize, color);
    }
}

bool SnakeController::CheckCollision() const
{
    // 벽 충돌
    if (m_transform.position.x < 0 || m_transform.position.x >= static_cast<float>(m_engineConfig.screenWidth) ||
        m_transform.position.y < 0 || m_transform.position.y >= static_cast<float>(m_engineConfig.screenHeight))
    {
        return true;
    }

    // 자기 몸 충돌
    for (const auto& segment : m_body)
    {
        if (m_transform.position == segment)
        {
            return true;
        }
    }

    return false;
}

void SnakeController::OnFoodEaten([[maybe_unused]] const FoodEatenEvent& event)
{
    m_shouldGrow = true;
    m_score++;
    m_eventSystem.Publish(ScoreChangedEvent{m_score});
}