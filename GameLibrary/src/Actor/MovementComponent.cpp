//
// Created by Labae on 1/11/26.
//

#include "Actor/MovementComponent.hpp"
namespace GameLibrary
{
    MovementComponent::MovementComponent(Actor* owner) : Component(owner), m_pawn(dynamic_cast<Pawn*>(owner)) {}
} // namespace GameLibrary