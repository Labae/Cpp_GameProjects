//
// Created by Labae on 1/11/26.
//

#ifndef MOVEMENTCOMPONENT_HPP
#define MOVEMENTCOMPONENT_HPP

#include "Component.hpp"
#include "Pawn.hpp"

#include <SFML/System/Vector2.hpp>

namespace GameLibrary
{

    class MovementComponent : public Component
    {
    public:
        explicit MovementComponent(Actor* owner);
        ~MovementComponent() override = default;

        MovementComponent(const MovementComponent&) = delete;
        MovementComponent(MovementComponent&&) = delete;
        MovementComponent& operator=(const MovementComponent&) = delete;
        MovementComponent& operator=(MovementComponent&&) = delete;

        void SetVelocity(const sf::Vector2f& velocity) noexcept { m_velocity = velocity; }
        [[nodiscard]] const sf::Vector2f& GetVelocity() const noexcept { return m_velocity; }

    protected:
        [[nodiscard]] Pawn* GetPawn() const noexcept { return m_pawn; }
        sf::Vector2f m_velocity{};

    private:
        Pawn* m_pawn{};
    };

} // namespace GameLibrary

#endif // MOVEMENTCOMPONENT_HPP
