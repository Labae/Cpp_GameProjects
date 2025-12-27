#pragma once

#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include "Component.hpp"

namespace GameLibrary
{
	class Transform;

	class GameObject
	{
	public:
		GameObject();
		~GameObject() = default;

		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) = default;
		GameObject& operator=(const GameObject&) = delete;
		GameObject& operator=(GameObject&&) = default;

		template<typename T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			auto component = std::make_unique<T>(std::forward<Args>(args)...);
			auto* ptr = component.get();

			m_componentMap[std::type_index(typeid(T))] = ptr;
			m_components.push_back(std::move(component));

			ptr->Init();

			return ptr;
		}

		template<typename T>
		[[nodiscard]] T* GetComponent() const noexcept
		{
			auto it = m_componentMap.find(std::type_index(typeid(T)));
			if (it == m_componentMap.end())
			{
				return nullptr;
			}
			return static_cast<T*>(it->second);
		}

		template<typename T>
		[[nodiscard]] bool HasComponent() const noexcept
		{
			return m_componentMap.contains(std::type_index(typeid(T)));
		}

		void Init();
		void Destroy();

		void Update(float deltaTime);
		void Render(IGraphics& graphics);

		[[nodiscard]] auto& GetTransform() const noexcept { return *m_transform; }

		[[nodiscard]] auto IsActive() const noexcept { return m_isActive; }
		void SetActive(bool active) noexcept { m_isActive = active; }

	private:
		Transform* m_transform;
		bool m_isActive{};

		std::vector<std::unique_ptr<Component>> m_components{};
		std::unordered_map<std::type_index, Component*> m_componentMap{};
	};
}