#include "GameObject/BoxCollider.hpp"

#include "Interfaces/IPhysicsSystem.hpp"

namespace GameLibrary
{
    BoxCollider::BoxCollider(Transform& transform, IPhysicsSystem& physics, int32_t width, int32_t height,
                             CollisionCallback callback)
        : m_transform(transform)
        , m_physics(physics)
        , m_width(width)
        , m_height(height)
        , m_onCollision(std::move(callback))
    {
    }

    void BoxCollider::Init()
    {
        m_physics.RegisterCollider(this);
    }

    void BoxCollider::Destroy()
    {
        m_physics.UnregisterCollider(this);
    }

    sf::IntRect BoxCollider::GetBounds() const
    {
        return {sf::Vector2i(m_transform.position.x, m_transform.position.y), sf::Vector2i(m_width, m_height)};
    }

    void BoxCollider::OnCollision(ICollidable* other)
    {
        if (m_onCollision != nullptr)
        {
            m_onCollision(other);
        }
    }
} // namespace GameLibrary