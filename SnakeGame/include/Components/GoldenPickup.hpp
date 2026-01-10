#pragma once

#include "../../../GameLibrary/include/Actor/Component.hpp"
#include "../../../GameLibrary/include/Actor/Transform.hpp"
#include "../../../GameLibrary/include/Services/EventService.hpp"

namespace GameLibrary
{
    class IGraphics;
    class ICollidable;
    class Actor;
} // namespace GameLibrary

class GoldenPickup final : public GameLibrary::Component
{
public:
    GoldenPickup(GameLibrary::Actor* owner, GameLibrary::EventService& eventSystem, int32_t gridSize,
                 float lifeTime = 3.0f);

    void Update(float deltaTime) override;
    void Render(GameLibrary::IGraphics& graphics) override;

    void OnCollision(GameLibrary::Actor* other);

    [[nodiscard]] bool IsExpired() const noexcept { return m_elapsedTime >= m_lifeTime; }
    [[nodiscard]] bool IsEaten() const noexcept { return m_isEaten; }

private:
    GameLibrary::EventService& m_eventSystem;

    int32_t m_gridSize{};
    float m_lifeTime{};
    float m_elapsedTime{};
    bool m_isEaten{};
};