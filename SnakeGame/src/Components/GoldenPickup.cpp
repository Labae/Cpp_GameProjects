#include "Components/GoldenPickup.hpp"

#include "Actor/Actor.hpp"
#include "Events/GameEvents.hpp"
#include "Interfaces/IGraphics.hpp"

GoldenPickup::GoldenPickup(GameLibrary::Actor* owner, GameLibrary::EventService& eventSystem, const int32_t gridSize,
                           const float lifeTime)
    : Component(owner)
    , m_eventSystem(eventSystem)
    , m_gridSize(gridSize)
    , m_lifeTime(lifeTime)
{
}

void GoldenPickup::Update(float deltaTime)
{
    if (m_isEaten)
    {
        return;
    }

    m_elapsedTime += deltaTime;
}

void GoldenPickup::Render(GameLibrary::IGraphics& graphics)
{
    if (m_isEaten || IsExpired())
    {
        return;
    }

    const float remaining = 1.0f - (m_elapsedTime / m_lifeTime);

    // 남은 시간 40% 이하면 깜빡임
    bool isVisible = true;
    if (remaining < 0.4f)
    {
        isVisible = static_cast<int32_t>(m_elapsedTime * 10) % 2 == 0;
    }

    if (isVisible)
    {
        const auto& transform = m_owner->GetTransform();
        graphics.FillRect(static_cast<int32_t>(transform.position.x), static_cast<int32_t>(transform.position.y),
                          m_gridSize, m_gridSize, sf::Color(255, 200, 0, 255));
    }
}

void GoldenPickup::OnCollision(const GameLibrary::Actor* other)
{
    if (other->GetTag() != "Snake")
    {
        return;
    }

    if (m_isEaten || IsExpired())
    {
        return;
    }

    m_isEaten = true;

    const auto& transform = m_owner->GetTransform();

    const float centerX = transform.position.x + static_cast<float>(m_gridSize) * 0.5f;
    const float centerY = transform.position.y + static_cast<float>(m_gridSize) * 0.5f;
    m_eventSystem.Publish(GoldenFoodEatenEvent{centerX, centerY});

    m_owner->Destroy();
}