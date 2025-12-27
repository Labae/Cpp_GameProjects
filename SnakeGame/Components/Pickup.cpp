#include "Pickup.hpp"
#include "Interfaces/IGraphics.hpp"
#include "Systems/EventSystem.hpp"
#include "Events/GameEvents.hpp"

Pickup::Pickup(GameLibrary::Transform& transform, GameLibrary::EventSystem& eventSystem, int gridSize)
    : m_transform(transform)
    , m_eventSystem(eventSystem)
    , m_gridSize(gridSize)
{
}

void Pickup::Init()
{
    m_snakeMovedToken = m_eventSystem.Subscribe<SnakeMovedEvent>(
        [this](const SnakeMovedEvent& event) { OnSnakeMoved(event); }
    );
}

void Pickup::Render(GameLibrary::IGraphics& graphics)
{
    graphics.FillRect(
        static_cast<int>(m_transform.position.x),
        static_cast<int>(m_transform.position.y),
        m_gridSize,
        m_gridSize,
        { 255, 0, 0, 255 }
    );
}

void Pickup::OnSnakeMoved(const SnakeMovedEvent& event)
{
    if (event.position == m_transform.position)
    {
        // 셀 중앙 좌표 계산
        float centerX = m_transform.position.x + m_gridSize / 2.0f;
        float centerY = m_transform.position.y + m_gridSize / 2.0f;
        m_eventSystem.Publish(FoodEatenEvent{ centerX, centerY });
    }
}