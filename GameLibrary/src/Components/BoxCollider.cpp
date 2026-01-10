#include "Components/BoxCollider.hpp"

#include "Components/Transform.hpp"
#include "Actor/Actor.hpp"
#include "Services/PhysicsService.hpp"

namespace GameLibrary
{
    BoxCollider::BoxCollider(Actor* owner, PhysicsService& physics, const int32_t width, const int32_t height,
                             CollisionCallback callback)
        : Component(owner)
        , m_physics(physics)
        , m_width(width)
        , m_height(height)
        , m_onCollision(std::move(callback))
    {
        m_physics.RegisterCollider(this);
    }
    BoxCollider::~BoxCollider()
    {
        m_physics.UnregisterCollider(this);
    }

    sf::IntRect BoxCollider::GetBounds() const
    {
        const auto& transform = m_owner->GetTransform();
        return {sf::Vector2i(transform.position.x, transform.position.y), sf::Vector2i(m_width, m_height)};
    }

    void BoxCollider::OnCollision(Actor* other)
    {
        if (not m_enabled)
        {
            return;
        }

        if (not m_onCollision)
        {
            return;
        }

        m_onCollision(other);
    }

    Actor* BoxCollider::GetActor() const
    {
        return m_owner;
    }
} // namespace GameLibrary