//
// Created by Labae on 1/11/26.
//

#ifndef PAWN_HPP
#define PAWN_HPP
#include "Actor.hpp"

#include <SFML/System/Vector2.hpp>

namespace GameLibrary
{
    class Pawn final : public Actor
    {
    public:
        explicit Pawn(Scene* scene);
        ~Pawn() override = default;

        Pawn(const Pawn&) = delete;
        Pawn(Pawn&&) = delete;
        Pawn& operator=(const Pawn&) = delete;
        Pawn& operator=(Pawn&&) = delete;

        void SetVelocity(const sf::Vector2f& velocity) noexcept { m_velocity = velocity; }
        [[nodiscard]] sf::Vector2f GetVelocity() const noexcept { return m_velocity; }

    protected:
        sf::Vector2f m_velocity{};
    };

} // namespace GameLibrary

#endif // PAWN_HPP
