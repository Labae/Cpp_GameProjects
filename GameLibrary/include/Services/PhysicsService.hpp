#pragma once

#include "Interfaces/ICollidable.hpp"

#include <unordered_map>
#include <vector>

namespace GameLibrary
{
    class PhysicsService
    {
    public:
        PhysicsService() = default;
        explicit PhysicsService(int32_t cellSize);
        ~PhysicsService() = default;

        PhysicsService(const PhysicsService&) = delete;
        PhysicsService(PhysicsService&&) = delete;
        PhysicsService& operator=(const PhysicsService&) = delete;
        PhysicsService& operator=(PhysicsService&&) = delete;

        void RegisterCollider(ICollidable* collider) noexcept;
        void UnregisterCollider(ICollidable* collider) noexcept;
        void CheckCollisions() noexcept;

        void SetCellSize(const int32_t cellSize) noexcept { m_cellSize = cellSize; }

    private:
        void RebuildSpatialHash();
        [[nodiscard]] static int64_t GetCellKey(int32_t x, int32_t y) noexcept;

        std::vector<ICollidable*> m_colliders{};
        std::unordered_map<int64_t, std::vector<size_t>> m_spatialHash{};
        int32_t m_cellSize{32};
    };
} // namespace GameLibrary