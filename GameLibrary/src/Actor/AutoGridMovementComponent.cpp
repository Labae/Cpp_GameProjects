//
// Created by Labae on 1/11/26.
//

#include "Actor/AutoGridMovementComponent.hpp"
namespace GameLibrary
{

    AutoGridMovementComponent::AutoGridMovementComponent(Actor* owner, const int32_t gridSize, const float moveInterval)
        : MovementComponent(owner)
        , m_gridSize(gridSize)
        , m_moveInterval(moveInterval)
    {
    }

    void AutoGridMovementComponent::Update(const float deltaTime)
    {
        m_movedThisFrame = false;

        if (!m_enabled)
        {
            return;
        }

        m_moveTimer += deltaTime;
        if (m_moveTimer >= m_moveInterval)
        {
            m_moveTimer = 0.0f;

            auto& transform = m_owner->GetTransform();
            transform.position.x += m_direction.x * static_cast<float>(m_gridSize);
            transform.position.y += m_direction.y * static_cast<float>(m_gridSize);

            m_movedThisFrame = true;
        }
    }

} // namespace GameLibrary