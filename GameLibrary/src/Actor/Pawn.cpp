//
// Created by Labae on 1/11/26.
//

#include "Actor/Pawn.hpp"
namespace GameLibrary
{
    Pawn::Pawn(Scene* scene) : Actor(scene) {}
    Pawn::~Pawn()
    {
        if (m_controller)
        {
            m_controller->UnPossess();
        }
    }

    void Pawn::AddMovementInput(const sf::Vector2f& direction)
    {
        m_pendingInput = direction;
    }

    sf::Vector2f Pawn::ConsumeMovementInput() noexcept
    {
        const auto input = m_pendingInput;
        m_pendingInput = {};
        return input;
    }

} // namespace GameLibrary