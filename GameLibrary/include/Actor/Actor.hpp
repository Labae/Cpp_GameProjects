//
// Created by Labae on 1/10/26.
//

#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "../Components/Transform.hpp"
#include "Interfaces/IGraphics.hpp"

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace GameLibrary
{
    class Component;
    class Transform;
    class Scene;

    class Actor
    {
        friend class Scene;

    public:
        explicit Actor(Scene* scene);
        virtual ~Actor() = default;

        Actor(const Actor&) = delete;
        Actor(Actor&&) = delete;
        Actor& operator=(const Actor&) = delete;
        Actor& operator=(Actor&&) = delete;

        template <typename T, typename... Args> T* AddComponent(Args&&... args)
        {
            auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
            auto* ptr = component.get();

            m_componentsMap[std::type_index(typeid(T))] = ptr;
            m_components.emplace_back(std::move(component));

            ptr->Init();

            return ptr;
        }

        template <typename T> T* GetComponent() const noexcept
        {
            const auto it = m_componentsMap.find(std::type_index(typeid(T)));
            if (it == m_componentsMap.end())
            {
                return nullptr;
            }
            return static_cast<T*>(it->second);
        }

        template <typename T> [[nodiscard]] bool HasComponent() const noexcept
        {
            return m_componentsMap.contains(std::type_index(typeid(T)));
        }

        virtual void OnCreate() {}
        virtual void OnDestroy() {}

        virtual void Update(const float deltaTime);
        virtual void Render(IGraphics& graphics);

        void Destroy();

        [[nodiscard]] Transform& GetTransform() const noexcept { return *m_transform; }

        [[nodiscard]] bool IsActive() const noexcept { return m_isActive; }
        void SetActive(const bool active) noexcept { m_isActive = active; }

        [[nodiscard]] bool IsUpdateEnabled() const noexcept { return m_updateEnabled; }
        void SetUpdateEnabled(const bool enabled) noexcept { m_updateEnabled = enabled; }

        [[nodiscard]] bool IsRenderEnabled() const noexcept { return m_renderEnabled; }
        void SetRenderEnabled(const bool enabled) noexcept { m_renderEnabled = enabled; }

        [[nodiscard]] bool IsPendingDestroy() const noexcept { return m_pendingDestroy; }

        void SetTag(const std::string& tag) noexcept { m_tag = tag; }
        [[nodiscard]] std::string GetTag() const noexcept { return m_tag; }

    protected:
        Transform* m_transform;
        Scene* const m_scene;

    private:
        void DestroyInternal();

        bool m_isActive{true};
        bool m_updateEnabled{true};
        bool m_renderEnabled{true};
        bool m_pendingDestroy{false};

        std::string m_tag{};

        std::vector<std::unique_ptr<Component>> m_components{};
        std::unordered_map<std::type_index, Component*> m_componentsMap{};
    };

} // GameLibrary

#endif //ACTOR_HPP
