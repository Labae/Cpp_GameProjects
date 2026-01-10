#include "Scene/Scene.hpp"

#include "Actor/Actor.hpp"
#include "Interfaces/IGraphics.hpp"

#include <algorithm>

namespace GameLibrary
{
    Scene::Scene(const std::string& name, ServiceContainer& container) noexcept : m_name(name), m_container(container)
    {
    }

    Actor* Scene::CreateActor()
    {
        m_actors.emplace_back(std::make_unique<Actor>(this));
        auto* ptr = m_actors.back().get();
        ptr->OnCreate();

        return ptr;
    }

    void Scene::DestroyActor(Actor* actor)
    {
        if (not actor)
        {
            return;
        }

        if (const auto it = std::ranges::find(m_pendingDestroy, actor); it == m_pendingDestroy.end())
        {
            m_pendingDestroy.emplace_back(actor);
        }
    }

    void Scene::Update(const float deltaTime)
    {
        for (const auto& actor : m_actors)
        {
            if (actor->IsActive() and actor->IsUpdateEnabled() and not actor->IsPendingDestroy())
            {
                actor->Update(deltaTime);
            }
        }

        ProcessPendingDestroy();
    }

    void Scene::Render(IGraphics& graphics)
    {
        for (const auto& actor : m_actors)
        {
            if (actor->IsActive() and actor->IsRenderEnabled())
            {
                actor->Render(graphics);
            }
        }
    }

    void Scene::Clear()
    {
        for (const auto& actor : m_actors)
        {
            actor->DestroyInternal();
        }
        m_actors.clear();
        m_pendingDestroy.clear();
    }

    void Scene::ProcessPendingDestroy()
    {
        for (auto* actor : m_pendingDestroy)
        {
            actor->DestroyInternal();
            auto it = std::ranges::find_if(m_actors, [actor](const auto& ptr) { return ptr.get() == actor; });
            if (it != m_actors.end())
            {
                m_actors.erase(it);
            }
        }
        m_pendingDestroy.clear();
    }
} // namespace GameLibrary