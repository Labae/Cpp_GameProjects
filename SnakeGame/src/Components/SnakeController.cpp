#include "Components/SnakeController.hpp"

#include "Actor/Actor.hpp"
#include "Config/SnakeGameConfig.hpp"
#include "Core/EngineConfig.hpp"
#include "Events/GameEvents.hpp"
#include "Interfaces/IGraphics.hpp"
#include "Interfaces/IInputProvider.hpp"

#include <algorithm>

SnakeController::SnakeController(GameLibrary::Actor* owner, GameLibrary::EventService& eventSystem,
                                 const GameLibrary::EngineConfig& engineConfig,
                                 const SnakeGame::SnakeGameConfig& gameConfig)
    : Component(owner)
    , m_eventService(eventSystem)
    , m_engineConfig(engineConfig)
    , m_gameConfig(gameConfig)
{
}

void SnakeController::Init()
{
    m_movement = m_owner->GetComponent<GameLibrary::GridMovementComponent>();
    m_gameOverToken = m_eventService.Subscribe<GameOverEvent>([this]([[maybe_unused]] const GameOverEvent& event)
                                                              { m_isGameOver = true; });
}

void SnakeController::Update([[maybe_unused]] const float deltaTime)
{
    if (m_isGameOver)
    {
        return;
    }

    if (not m_movement)
    {
        return;
    }

    if (not m_movement->HasMovedThisFrame())
    {
        return;
    }

    const auto& transform = m_owner->GetTransform();

    const auto direction = m_movement->GetDirection();
    const sf::Vector2f previousPosition = {
        transform.position.x - direction.x * static_cast<float>(m_gameConfig.gridSize),
        transform.position.y - direction.y * static_cast<float>(m_gameConfig.gridSize)};

    m_body.insert(m_body.begin(), previousPosition);

    if (m_shouldGrow)
    {
        m_shouldGrow = false;
    }
    else if (!m_body.empty())
    {
        m_body.pop_back();
    }

    if (CheckCollision())
    {
        m_eventService.Publish(GameOverEvent{});
        return;
    }

    m_eventService.Publish(SnakeMovedEvent{transform.position});
}

void SnakeController::Render(GameLibrary::IGraphics& graphics)
{
    const auto totalSegmentCount = static_cast<int32_t>(m_body.size()) + 1;
    const auto& transform = m_owner->GetTransform();

    // 머리 그리기
    graphics.FillRect(static_cast<int32_t>(transform.position.x), static_cast<int32_t>(transform.position.y),
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

void SnakeController::OnCollision([[maybe_unused]] GameLibrary::Actor* other)
{
    if (m_isGameOver)
    {
        return;
    }

    m_shouldGrow = true;
}

bool SnakeController::CheckCollision() const
{
    const auto& transform = m_owner->GetTransform();
    // 벽 충돌
    if (transform.position.x < 0 || transform.position.x >= static_cast<float>(m_engineConfig.screenWidth) ||
        transform.position.y < 0 || transform.position.y >= static_cast<float>(m_engineConfig.screenHeight))
    {
        return true;
    }

    // 자기 몸 충돌
    for (const auto& segment : m_body)
    {
        if (transform.position == segment)
        {
            return true;
        }
    }

    return false;
}