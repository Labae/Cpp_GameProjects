//
// Created by Labae on 1/11/26.
//

#ifndef AUTOGRIDMOVEMENTCOMPONENT_HPP
#define AUTOGRIDMOVEMENTCOMPONENT_HPP
#include "MovementComponent.hpp"

namespace GameLibrary
{

    class AutoGridMovementComponent : public MovementComponent
    {
    public:
        explicit AutoGridMovementComponent(Actor* owner, const int32_t gridSize, const float moveInterval);

        void Update(float deltaTime) override;

        void SetDirection(const sf::Vector2f direction) { m_direction = direction; }
        [[nodiscard]] const sf::Vector2f& GetDirection() const noexcept { return m_direction; }
        [[nodiscard]] bool HasMovedThisFrame() const noexcept { return m_movedThisFrame; }

    private:
        int32_t m_gridSize{};
        float m_moveInterval{};
        float m_moveTimer{};
        sf::Vector2f m_direction{1.0f, 0.0f};
        bool m_movedThisFrame{};
    };

} // namespace GameLibrary

#endif // AUTOGRIDMOVEMENTCOMPONENT_HPP
