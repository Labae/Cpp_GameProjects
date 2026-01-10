//
// Created by Labae on 1/11/26.
//

#include "Actor/GridMovementComponent.hpp"
namespace GameLibrary
{
    GridMovementComponent::GridMovementComponent(Actor* owner, const int32_t gridSize, const float moveInterval)
        : MovementComponent(owner)
        , m_gridSize(gridSize)
        , m_moveInterval(moveInterval)
    {
    }

    void GridMovementComponent::Update(const float deltaTime)
    {
        m_movedThisFrame = false;

        if (not m_enabled)
        {
            return;
        }

        auto* pawn = GetPawn();
        if (not pawn)
        {
            return;
        }

        if (const auto input = pawn->ConsumeMovementInput(); input != sf::Vector2f{})
        {
            m_direction = input;
        }

        m_moveTimer += deltaTime;
        if (m_moveTimer >= m_moveInterval)
        {
            m_moveTimer = 0.0f;

            auto& transform = pawn->GetTransform();
            transform.position.x += m_direction.x * static_cast<float>(m_gridSize);
            transform.position.y += m_direction.y * static_cast<float>(m_gridSize);

            m_movedThisFrame = true;
        }
    }
} // namespace GameLibrary