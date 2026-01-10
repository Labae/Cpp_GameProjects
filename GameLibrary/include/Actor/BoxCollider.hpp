#pragma once

#include "Actor/Component.hpp"
#include "Interfaces/ICollidable.hpp"

#include <functional>

namespace GameLibrary
{
    class PhysicsService;

    class BoxCollider final : public Component, public ICollidable
    {
    public:
        using CollisionCallback = std::function<void(Actor*)>;

        BoxCollider(Actor* owner, PhysicsService& physics, int32_t width, int32_t height,
                    CollisionCallback callback);
        ~BoxCollider() override;

        [[nodiscard]] sf::IntRect GetBounds() const override;
        void OnCollision(Actor* other) override;
        [[nodiscard]] Actor* GetActor() const override;

    private:
        PhysicsService& m_physics;
        int32_t m_width{};
        int32_t m_height{};
        CollisionCallback m_onCollision{};
    };
} // namespace GameLibrary
