//
// Created by Labae on 1/11/26.
//

#include "Actor/LinearMovementComponent.hpp"
namespace GameLibrary
{

    LinearMovementComponent::LinearMovementComponent(Actor* owner) : MovementComponent(owner) {}
    void LinearMovementComponent::Update(const float deltaTime)
    {
        if (not m_enabled)
        {
            return;
        }

        auto& transform = m_owner->GetTransform();
        transform.position.x += m_velocity.x * deltaTime;
        transform.position.y += m_velocity.y * deltaTime;
    }

} // namespace GameLibrary