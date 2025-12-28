#pragma once

#include "GameObject/GameObject.hpp"

#include <memory>
#include <string>
#include <vector>

namespace GameLibrary
{
    class IGraphics;
    class ServiceContainer;

    class Scene
    {
    public:
        Scene(const std::string& name, ServiceContainer& container) noexcept;
        virtual ~Scene() = default;

        Scene(const Scene&) = delete;
        Scene(Scene&&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene& operator=(Scene&&) = delete;

        virtual void OnEnter() {}
        virtual void OnExit() {}

        [[nodiscard]] GameObject* CreateGameObject();
        void DestroyGameObject(GameObject* gameObject);

        virtual void Update(float deltaTime);
        virtual void Render(IGraphics& graphics);
        void Clear();

        [[nodiscard]] const std::string& GetName() const noexcept { return m_name; }
        [[nodiscard]] ServiceContainer& GetContainer() noexcept { return m_container; }

    private:
        std::string m_name{};
        ServiceContainer& m_container;
        std::vector<std::unique_ptr<GameObject>> m_gameObjects{};
        std::vector<GameObject*> m_pendingDestroy{};
    };
} // namespace GameLibrary