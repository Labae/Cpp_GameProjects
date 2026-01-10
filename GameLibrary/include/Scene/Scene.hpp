#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Actor/Actor.hpp"

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

        Actor* CreateActor();
        template <typename T, typename... Args>
        T* CreateActor(Args&&... args)
        {
            auto actor = std::make_unique<T>(this, std::forward<Args>(args)...);
            auto* ptr = actor.get();

            ptr->OnCreate();
            m_actors.emplace_back(std::move(actor));

            return ptr;
        }

        void DestroyActor(Actor* actor);

        virtual void Update(float deltaTime);
        virtual void Render(IGraphics& graphics);
        void Clear();

        [[nodiscard]] const std::string& GetName() const noexcept { return m_name; }
        [[nodiscard]] ServiceContainer& GetContainer() const noexcept { return m_container; }

    private:
        void ProcessPendingDestroy();

        std::string m_name{};
        ServiceContainer& m_container;
        std::vector<std::unique_ptr<Actor>> m_actors{};
        std::vector<Actor*> m_pendingDestroy{};
    };
} // namespace GameLibrary