#include "Components/Pickup.hpp"

#include "Events/GameEvents.hpp"
#include "Interfaces/IGraphics.hpp"

Pickup::Pickup(GameLibrary::Transform& transform, GameLibrary::EventSystem& eventSystem, int32_t gridSize)
    : m_transform(transform)
    , m_eventSystem(eventSystem)
    , m_gridSize(gridSize)
{
}

void Pickup::Render(GameLibrary::IGraphics& graphics)
{
    graphics.FillRect(static_cast<int32_t>(m_transform.position.x), static_cast<int32_t>(m_transform.position.y),
                      m_gridSize, m_gridSize, sf::Color(255, 0, 0, 255));
}

void Pickup::OnCollision([[maybe_unused]]GameLibrary::ICollidable* other)
{
    const float centerX = m_transform.position.x + static_cast<float>(m_gridSize) * 0.5f;
    const float centerY = m_transform.position.y + static_cast<float>(m_gridSize) * 0.5f;
    m_eventSystem.Publish(FoodEatenEvent{centerX, centerY});
}