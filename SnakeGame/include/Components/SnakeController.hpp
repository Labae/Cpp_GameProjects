#pragma once

#include "GameObject/Component.hpp"
#include "GameObject/Transform.hpp"
#include "Systems/EventSystem.hpp"

#include <SFML/System/Vector2.hpp>
#include <vector>

namespace GameLibrary
{
    class IInputProvider;
    class IGraphics;
    class ICollidable;
    struct EngineConfig;
} // namespace GameLibrary

namespace SnakeGame
{
    struct SnakeGameConfig;
}

class SnakeController : public GameLibrary::Component
{
public:
    SnakeController(GameLibrary::Transform& transform, GameLibrary::IInputProvider& input,
                    GameLibrary::EventSystem& eventSystem, const GameLibrary::EngineConfig& engineConfig,
                    const SnakeGame::SnakeGameConfig& gameConfig);

    void Init() override;
    void Update(float deltaTime) override;
    void Render(GameLibrary::IGraphics& graphics) override;

    void OnCollision(GameLibrary::ICollidable* other);

private:
    [[nodiscard]] bool CheckCollision() const;

    GameLibrary::Transform& m_transform;
    GameLibrary::IInputProvider& m_input;
    GameLibrary::EventSystem& m_eventSystem;
    const GameLibrary::EngineConfig& m_engineConfig;
    const SnakeGame::SnakeGameConfig& m_gameConfig;

    GameLibrary::SubscriptionToken m_gameOverToken{};

    std::vector<sf::Vector2f> m_body{};
    sf::Vector2f m_direction{1.0f, 0.0f};
    float m_moveTimer{};
    bool m_shouldGrow{};

    // 이것도 그럼.. 그냥 Component에 IsEnable를 추가하는게 좋을듯?
    bool m_isGameOver{};
};