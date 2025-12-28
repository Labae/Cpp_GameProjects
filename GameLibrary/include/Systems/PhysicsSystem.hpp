#pragma once

#include "Interfaces/ICollidable.hpp"
#include "Interfaces/IPhysicsSystem.hpp"

#include <unordered_map>
#include <vector>

namespace GameLibrary
{
    class PhysicsSystem : public IPhysicsSystem
    {
    public:
        PhysicsSystem() = default;
        explicit PhysicsSystem(int32_t cellSize);
        ~PhysicsSystem() = default;

        PhysicsSystem(const PhysicsSystem&) = delete;
        PhysicsSystem(PhysicsSystem&&) = delete;
        PhysicsSystem& operator=(const PhysicsSystem&) = delete;
        PhysicsSystem& operator=(PhysicsSystem&&) = delete;

        void RegisterCollider(ICollidable* collider) noexcept override;
        void UnregisterCollider(ICollidable* collider) noexcept override;
        void CheckCollisions() noexcept override;

        void SetCellSize(int32_t cellSize) noexcept { m_cellSize = cellSize; }

    private:
        void RebuildSpatialHash();
        [[nodiscard]] int64_t GetCellKey(int32_t x, int32_t y) const noexcept;

        std::vector<ICollidable*> m_colliders{};
        std::unordered_map<int64_t, std::vector<size_t>> m_spatialHash{};
        int32_t m_cellSize{32};
    };
} // namespace GameLibrary