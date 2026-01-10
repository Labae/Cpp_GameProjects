//
// Created by Labae on 1/11/26.
//

#ifndef GRIDMOVEMENTCOMPONENT_HPP
#define GRIDMOVEMENTCOMPONENT_HPP
#include "../Components/MovementComponent.hpp"

namespace GameLibrary
{

    class GridMovementComponent final : public MovementComponent
    {
    public:
        GridMovementComponent(Actor* owner, const int32_t gridSize, const float moveInterval);

        void Update(float deltaTime) override;

        [[nodiscard]] sf::Vector2f GetDirection() const noexcept { return m_direction; }
        [[nodiscard]] bool HasMovedThisFrame() const noexcept { return m_movedThisFrame; }

    private:
        int32_t m_gridSize{};
        float m_moveInterval{};
        float m_moveTimer{};
        sf::Vector2f m_direction{};
        bool m_movedThisFrame{};
    };

} // namespace GameLibrary

#endif // GRIDMOVEMENTCOMPONENT_HPP
