#pragma once

#include "Component.hpp"
#include "Interfaces/ICollidable.hpp"
#include "Transform.hpp"

#include <functional>

namespace GameLibrary
{
    class IPhysicsSystem;

    class BoxCollider : public Component, public ICollidable
    {
    public:
        using CollisionCallback = std::function<void(ICollidable*)>;

        BoxCollider(Transform& transform, IPhysicsSystem& physics, int32_t width, int32_t height,
                    CollisionCallback callback);

        void Init() override;
        void Destroy() override;

        [[nodiscard]] sf::IntRect GetBounds() const override;
        void OnCollision(ICollidable* other) override;

    private:
        Transform& m_transform;
        IPhysicsSystem& m_physics;
        int32_t m_width{};
        int32_t m_height{};
        CollisionCallback m_onCollision{};
    };
} // namespace GameLibrary
