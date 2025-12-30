#include "Components/GoldenPickup.hpp"

#include "Events/GameEvents.hpp"
#include "Interfaces/IGraphics.hpp"

GoldenPickup::GoldenPickup(GameLibrary::Transform& transform, GameLibrary::EventSystem& eventSystem, int32_t gridSize,
                           float lifeTime)
    : m_transform(transform)
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
        graphics.FillRect(static_cast<int32_t>(m_transform.position.x), static_cast<int32_t>(m_transform.position.y),
                          m_gridSize, m_gridSize, sf::Color(255, 200, 0, 255));
    }
}

void GoldenPickup::OnCollision([[maybe_unused]]GameLibrary::ICollidable* other)
{
    if (m_isEaten || IsExpired())
    {
        return;
    }

    m_isEaten = true;

    const float centerX = m_transform.position.x + static_cast<float>(m_gridSize) * 0.5f;
    const float centerY = m_transform.position.y + static_cast<float>(m_gridSize) * 0.5f;
    m_eventSystem.Publish(GoldenFoodEatenEvent{centerX, centerY});
}