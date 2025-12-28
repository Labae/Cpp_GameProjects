#include "Systems/PhysicsSystem.hpp"

#include <algorithm>
#include <set>

namespace GameLibrary
{
    PhysicsSystem::PhysicsSystem(int32_t cellSize) : m_cellSize(cellSize) {}

    void PhysicsSystem::RegisterCollider(ICollidable* collider) noexcept
    {
        if (collider)
        {
            m_colliders.emplace_back(collider);
        }
    }

    void PhysicsSystem::UnregisterCollider(ICollidable* collider) noexcept
    {
        auto it = std::find(m_colliders.begin(), m_colliders.end(), collider);
        if (it != m_colliders.end())
        {
            m_colliders.erase(it);
        }
    }

    void PhysicsSystem::CheckCollisions() noexcept
    {
        RebuildSpatialHash();

        std::set<std::pair<size_t, size_t>> checkedPairs;

        for (const auto& [cellKey, indices] : m_spatialHash)
        {
            for (size_t i = 0; i < indices.size(); ++i)
            {
                for (size_t j = i + 1; j < indices.size(); ++j)
                {
                    size_t indexA = indices[i];
                    size_t indexB = indices[j];

                    // index 정렬(작은순으로)
                    size_t first = std::min(indexA, indexB);
                    size_t second = std::max(indexA, indexB);

                    // 이미 체크했으면 스킵
                    if (checkedPairs.contains({first, second}))
                    {
                        continue;
                    }

                    checkedPairs.insert({first, second});

                    // 충돌 검사
                    auto* a = m_colliders[indexA];
                    auto* b = m_colliders[indexB];

                    if (a->GetBounds().findIntersection(b->GetBounds()))
                    {
                        a->OnCollision(b);
                        b->OnCollision(a);
                    }
                }
            }
        }
    }

    void PhysicsSystem::RebuildSpatialHash()
    {
        m_spatialHash.clear();

        for (size_t i = 0; i < m_colliders.size(); ++i)
        {
            const auto bounds = m_colliders[i]->GetBounds();

            const int32_t minCellX = bounds.position.x / m_cellSize;
            const int32_t minCellY = bounds.position.y / m_cellSize;
            const int32_t maxCellX = (bounds.position.x + bounds.size.x) / m_cellSize;
            const int32_t maxCellY = (bounds.position.y + bounds.size.y) / m_cellSize;

            for (int32_t cx = minCellX; cx <= maxCellX; ++cx)
            {
                for (int32_t cy = minCellY; cy <= maxCellY; ++cy)
                {
                    int64_t key = GetCellKey(cx, cy);
                    m_spatialHash[key].emplace_back(i);
                }
            }
        }
    }

    int64_t PhysicsSystem::GetCellKey(int32_t x, int32_t y) const noexcept
    {
        return (static_cast<int64_t>(y) << 32) | static_cast<int32_t>(x);
    }
} // namespace GameLibrary
