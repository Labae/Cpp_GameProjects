//
// Created by Labae on 1/11/26.
//

#ifndef LINEARMOVEMENTCOMPONENT_HPP
#define LINEARMOVEMENTCOMPONENT_HPP
#include "MovementComponent.hpp"

namespace GameLibrary
{

    class LinearMovementComponent final : public MovementComponent
    {
    public:
        explicit LinearMovementComponent(Actor* owner);

        void Update(float deltaTime) override;
    };

} // namespace GameLibrary

#endif // LINEARMOVEMENTCOMPONENT_HPP
