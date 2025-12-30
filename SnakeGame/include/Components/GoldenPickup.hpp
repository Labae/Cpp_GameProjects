#pragma once

#include "GameObject/Component.hpp"
#include "GameObject/Transform.hpp"
#include "Systems/EventSystem.hpp"

namespace GameLibrary
{
    class IGraphics;
    class ICollidable;
} // namespace GameLibrary

class GoldenPickup : public GameLibrary::Component
{
public:
    GoldenPickup(GameLibrary::Transform& transform, GameLibrary::EventSystem& eventSystem, int32_t gridSize,
                 float lifeTime = 3.0f);

    void Update(float deltaTime) override;
    void Render(GameLibrary::IGraphics& graphics) override;

    void OnCollision(GameLibrary::ICollidable* other);

    [[nodiscard]] bool IsExpired() const noexcept { return m_elapsedTime >= m_lifeTime; }
    [[nodiscard]] bool IsEaten() const noexcept { return m_isEaten; }

private:
    GameLibrary::Transform& m_transform;
    GameLibrary::EventSystem& m_eventSystem;

    int32_t m_gridSize{};
    float m_lifeTime{};
    float m_elapsedTime{};
    bool m_isEaten{};
};