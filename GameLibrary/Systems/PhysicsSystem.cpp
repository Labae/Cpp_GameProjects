#include "PhysicsSystem.hpp"
#include <algorithm>

namespace GameLibrary
{
	void PhysicsSystem::RegisterCollider(ICollidable* collider) noexcept
	{
		if (collider)
		{
			m_colliders.push_back(collider);
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
		for (size_t i = 0; i < m_colliders.size(); ++i)
		{
			for (size_t j = i + 1; j < m_colliders.size(); ++j)
			{
				auto* a = m_colliders[i];
				auto* b = m_colliders[j];

				if (Intersects(a->GetBounds(), b->GetBounds()))
				{
					a->OnCollision(b);
					b->OnCollision(a);
				}
			}
		}
	}

	bool PhysicsSystem::Intersects(const Bounds& a, const Bounds& b) noexcept
	{
		return a.x < b.x + b.width &&
			a.x + a.width > b.x &&
			a.y < b.y + b.height &&
			a.y + a.height > b.y;
	}
}