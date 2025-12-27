#pragma once

#include <vector>
#include <memory>
#include <string>

namespace GameLibrary
{
    class GameObject;
    class IGraphics;
    class ServiceContainer;

    class Scene
    {
    public:
        Scene(const std::string& name, ServiceContainer& container) noexcept;
        virtual ~Scene();

        Scene(const Scene&) = delete;
        Scene(Scene&&) = default;
        Scene& operator=(const Scene&) = delete;
        Scene& operator=(Scene&&) = default;

        virtual void OnEnter() {}
        virtual void OnExit() {}

        GameObject* CreateGameObject();
        void DestroyGameObject(GameObject* gameObject);

        virtual void Update(float deltaTime);
        virtual void Render(IGraphics& graphics);
        void Clear();

        [[nodiscard]] const std::string& GetName() const noexcept { return m_name; }
        [[nodiscard]] ServiceContainer& GetContainer() noexcept { return m_container; }

    private:
        std::string m_name;
        ServiceContainer& m_container;
        std::vector<std::unique_ptr<GameObject>> m_gameObjects;
        std::vector<GameObject*> m_pendingDestroy;
    };
}