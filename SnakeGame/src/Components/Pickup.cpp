#include "Components/Pickup.hpp"

#include "Events/GameEvents.hpp"
#include "Interfaces/IGraphics.hpp"

Pickup::Pickup(GameLibrary::Transform& transform, GameLibrary::EventSystem& eventSystem, int32_t gridSize)
    : m_transform(transform)
    , m_eventSystem(eventSystem)
    , m_gridSize(gridSize)
{
}

void Pickup::Init()
{
    m_snakeMovedToken =
        m_eventSystem.Subscribe<SnakeMovedEvent>([this](const SnakeMovedEvent& event) { OnSnakeMoved(event); });
}

void Pickup::Render(GameLibrary::IGraphics& graphics)
{
    graphics.FillRect(static_cast<int32_t>(m_transform.position.x), static_cast<int32_t>(m_transform.position.y),
                      m_gridSize, m_gridSize, sf::Color(255, 0, 0, 255));
}

void Pickup::OnSnakeMoved(const SnakeMovedEvent& event)
{
    if (event.position == m_transform.position)
    {
        const float centerX = m_transform.position.x + static_cast<float>(m_gridSize) / 2.0f;
        const float centerY = m_transform.position.y + static_cast<float>(m_gridSize) / 2.0f;
        m_eventSystem.Publish(FoodEatenEvent{centerX, centerY});
    }
}