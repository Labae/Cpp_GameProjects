#include "Components/Pickup.hpp"

#include "Events/GameEvents.hpp"
#include "Interfaces/IGraphics.hpp"
#include "Actor/Actor.hpp"

Pickup::Pickup(GameLibrary::Actor* owner, GameLibrary::EventService& eventSystem, const int32_t gridSize)
    : Component(owner)
    , m_eventSystem(eventSystem)
    , m_gridSize(gridSize)
{
}

void Pickup::Render(GameLibrary::IGraphics& graphics)
{
    const auto& transform = m_owner->GetTransform();
    graphics.FillRect(static_cast<int32_t>(transform.position.x), static_cast<int32_t>(transform.position.y),
                      m_gridSize, m_gridSize, sf::Color(255, 0, 0, 255));
}

void Pickup::OnCollision([[maybe_unused]]GameLibrary::Actor* other) const
{
    const auto& transform = m_owner->GetTransform();
    const float centerX = transform.position.x + static_cast<float>(m_gridSize) * 0.5f;
    const float centerY = transform.position.y + static_cast<float>(m_gridSize) * 0.5f;
    m_eventSystem.Publish(FoodEatenEvent{centerX, centerY});
}