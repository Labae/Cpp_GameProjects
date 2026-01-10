//
// Created by Labae on 1/11/26.
//

#ifndef PAWN_HPP
#define PAWN_HPP
#include "Actor.hpp"
#include "Controller.hpp"

#include <SFML/System/Vector2.hpp>

namespace GameLibrary
{
    class Controller;

    class Pawn final : public Actor
    {
    public:
        explicit Pawn(Scene* scene);
        ~Pawn() override;

        Pawn(const Pawn&) = delete;
        Pawn(Pawn&&) = delete;
        Pawn& operator=(const Pawn&) = delete;
        Pawn& operator=(Pawn&&) = delete;

        void SetController(Controller* controller) noexcept { m_controller = controller; }
        [[nodiscard]] Controller* GetController() const noexcept { return m_controller; }

        virtual void AddMovementInput(const sf::Vector2f& direction);
        [[nodiscard]] const sf::Vector2f& GetPendingInput() const noexcept { return m_pendingInput; }
        [[nodiscard]] sf::Vector2f ConsumeMovementInput() noexcept;


    protected:
        Controller* m_controller{};
        sf::Vector2f m_pendingInput{};
    };

} // namespace GameLibrary

#endif // PAWN_HPP
