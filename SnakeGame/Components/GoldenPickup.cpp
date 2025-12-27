#include "GoldenPickup.hpp"

#include "Events/GameEvents.hpp"
#include "Interfaces/IGraphics.hpp"
#include <cmath>

GoldenPickup::GoldenPickup(GameLibrary::Transform& transform, 
	GameLibrary::EventSystem& eventSystem,
	int32_t gridSize,
	float lifeTime)
	: m_transform(transform)
	, m_eventSystem(eventSystem)
	, m_gridSize(gridSize)
	, m_lifeTime(lifeTime)
{
}

void GoldenPickup::Init()
{
	m_snakeMovedToken = m_eventSystem.Subscribe<SnakeMovedEvent>(
		[this](const SnakeMovedEvent& event) {OnSnakeMoved(event); }
	);
}

void GoldenPickup::Update(float deltaTime)
{
	if (!m_isActive)
	{
		return;
	}

	m_elapsedTime += deltaTime;
	if (m_elapsedTime >= m_lifeTime)
	{
		Despawn();
	}
}

void GoldenPickup::Render(GameLibrary::IGraphics& graphics)
{
	if (!m_isActive)
	{
		return;
	}

	// 남은 시간
	float remaining = 1.0f - (m_elapsedTime / m_lifeTime);

	// 남은 시간 0.4퍼 이하면 Blink
	bool isVisible = true;
	if (remaining < 0.4f)
	{
		isVisible = static_cast<int32_t>(m_elapsedTime * 10) % 2 == 0;
	}

	if (isVisible)
	{
		GameLibrary::Color color = { 255, 200, 0, 255 };
		graphics.FillRect(
			static_cast<int32_t>(m_transform.position.x),
			static_cast<int32_t>(m_transform.position.y),
			m_gridSize,
			m_gridSize,
			color
		);
	}
}

void GoldenPickup::Spawn(float x, float y)
{
	m_transform.position = { x, y };
	m_elapsedTime = 0.0f;
	m_isActive = true;
}

void GoldenPickup::Despawn()
{
	m_isActive = false;
}


void GoldenPickup::OnSnakeMoved(const SnakeMovedEvent& event)
{
	if (m_isActive && event.position == m_transform.position)
	{
		// 셀 중앙 좌표 계산
		float centerX = m_transform.position.x + m_gridSize / 2.0f;
		float centerY = m_transform.position.y + m_gridSize / 2.0f;
		m_eventSystem.Publish(GoldenFoodEatenEvent{ centerX, centerY });
		Despawn();
	}
}