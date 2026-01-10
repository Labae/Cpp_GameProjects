//
// Created by Labae on 1/11/26.
//

#include "Components/MovingPickup.hpp"

#include "Actor/Actor.hpp"
#include "Actor/AutoGridMovementComponent.hpp"
#include "Core/EngineConfig.hpp"
#include "Events/GameEvents.hpp"
#include "Interfaces/IGraphics.hpp"
#include "Services/EventService.hpp"

MovingPickup::MovingPickup(GameLibrary::Actor* owner, GameLibrary::EventService& eventService,
                           const GameLibrary::EngineConfig& engineConfig, const int32_t gridSize, int32_t score)
    : Component(owner)
    , m_eventService(eventService)
    , m_engineConfig(engineConfig)
    , m_gridSize(gridSize)
    , m_score(score)
{
}
void MovingPickup::Init()
{
    m_movement = m_owner->GetComponent<GameLibrary::AutoGridMovementComponent>();
}
void MovingPickup::Update([[maybe_unused]] const float deltaTime)
{
    if (m_isEaten)
    {
        return;
    }

    BounceOffWalls();
}
void MovingPickup::Render(GameLibrary::IGraphics& graphics)
{
    if (m_isEaten)
    {
        return;
    }

    const auto& transform = m_owner->GetTransform();
    graphics.FillRect(static_cast<int32_t>(transform.position.x), static_cast<int32_t>(transform.position.y),
                      m_gridSize, m_gridSize, sf::Color(0, 200, 255, 255));
}
void MovingPickup::OnCollision(const GameLibrary::Actor* other)
{
    if (other->GetTag() != "Snake")
    {
        return;
    }

    if (m_isEaten)
    {
        return;
    }

    m_isEaten = true;

    const auto& transform = m_owner->GetTransform();

    const float centerX = transform.position.x + static_cast<float>(m_gridSize) * 0.5f;
    const float centerY = transform.position.y + static_cast<float>(m_gridSize) * 0.5f;
    m_eventService.Publish(GoldenFoodEatenEvent{centerX, centerY});

    m_owner->Destroy();
}

void MovingPickup::BounceOffWalls() const
{
    if (not m_movement or not m_movement->HasMovedThisFrame())
    {
        return;
    }

    const auto& transform = m_owner->GetTransform();
    auto direction = m_movement->GetDirection();
    bool bounced = false;

    if (transform.position.x <= 0 or
        transform.position.x >= static_cast<float>(m_engineConfig.screenWidth - m_gridSize))
    {
        direction.x *= -1;
        bounced = true;
    }

    if (transform.position.y <= 0 or
        transform.position.y >= static_cast<float>(m_engineConfig.screenHeight - m_gridSize))
    {
        direction.y *= -1;
        bounced = true;
    }

    if (bounced)
    {
        m_movement->SetDirection(direction);
    }
}