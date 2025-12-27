#pragma once

#include <vector>
#include "GameObject/Component.hpp"
#include "GameObject/Transform.hpp"
#include "Systems/EventSystem.hpp"

namespace GameLibrary
{
	class IInputProvider;
	class IGraphics;
	struct EngineConfig;
}

namespace SnakeGame
{
	struct SnakeGameConfig;
}

class SnakeController : public GameLibrary::Component
{
public:
	SnakeController(GameLibrary::Transform& transform,
		GameLibrary::IInputProvider& input,
		GameLibrary::EventSystem& eventSystem,
		const GameLibrary::EngineConfig& engineConfig,
		const SnakeGame::SnakeGameConfig& gameConfig);

	void Init() override;
	void Update(float deltaTime) override;
	void Render(GameLibrary::IGraphics& graphics) override;

private:
	void OnFoodEaten(const struct FoodEatenEvent& event);
	[[nodiscard]] bool CheckCollision() const;

	GameLibrary::Transform& m_transform;
	GameLibrary::IInputProvider& m_input;
	GameLibrary::EventSystem& m_eventSystem;
	const GameLibrary::EngineConfig& m_engineConfig;
	const SnakeGame::SnakeGameConfig& m_gameConfig;

	GameLibrary::SubscriptionToken m_foodEatenToken{};
	GameLibrary::SubscriptionToken m_gameOverToken{};

	std::vector<GameLibrary::Vector2> m_body{};
	GameLibrary::Vector2 m_direction = { 1.0f, 0.0f };
	float m_moveTimer{};
	int32_t m_score{};
	bool m_shouldGrow{};
	bool m_isGameOver{};
};