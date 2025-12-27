#pragma once

#include <vector>
#include "Interfaces/IPhysicsSystem.hpp"
#include "Interfaces/ICollidable.hpp"

namespace GameLibrary
{
    class PhysicsSystem : public IPhysicsSystem
    {
    public:
        void RegisterCollider(ICollidable* collider) noexcept override;
        void UnregisterCollider(ICollidable* collider) noexcept override;
        void CheckCollisions() noexcept override;

    private:
        [[nodiscard]] static bool Intersects(const Bounds& a, const Bounds& b) noexcept;

        std::vector<ICollidable*> m_colliders;
    };
}