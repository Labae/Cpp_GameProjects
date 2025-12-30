#pragma once

#include "GameObject/Component.hpp"
#include "GameObject/Transform.hpp"
#include "Systems/EventSystem.hpp"

namespace GameLibrary
{
    class IGraphics;
}

class GoldenPickup : public GameLibrary::Component
{
public:
    GoldenPickup(GameLibrary::Transform& transform, GameLibrary::EventSystem& eventSystem, int32_t gridSize,
                 float lifeTime = 3.0f);

    void Init() override;
    void Update(float deltaTime) override;
    void Render(GameLibrary::IGraphics& graphics) override;

    void Spawn(float x, float y);
    void Despawn();
    [[nodiscard]] bool IsActive() const noexcept { return m_isActive; }

private:
    void OnSnakeMoved(const struct SnakeMovedEvent& event);

    GameLibrary::Transform& m_transform;
    GameLibrary::EventSystem& m_eventSystem;
    GameLibrary::SubscriptionToken m_snakeMovedToken{};

    int32_t m_gridSize{};
    bool m_isActive{};
    float m_lifeTime{};
    float m_elapsedTime{};
};