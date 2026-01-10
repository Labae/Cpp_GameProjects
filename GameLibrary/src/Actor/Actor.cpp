//
// Created by Labae on 1/10/26.
//

#include "Actor/Actor.hpp"

#include "Components/Component.hpp"
#include "Scene/Scene.hpp"

namespace GameLibrary
{
    Actor::Actor(Scene* scene) : m_transform(nullptr), m_scene(scene)
    {
        m_transform = AddComponent<Transform>();
    }

    void Actor::Update(const float deltaTime)
    {
        for (const auto& component : m_components)
        {
            component->Update(deltaTime);
        }
    }

    void Actor::Render(IGraphics& graphics)
    {
        for (const auto& component : m_components)
        {
            component->Render(graphics);
        }
    }
    void Actor::Destroy()
    {
        if (m_pendingDestroy)
        {
            return;
        }

        m_pendingDestroy = true;



        if (m_scene)
        {
            m_scene->DestroyActor(this);
        }
    }

    void Actor::DestroyInternal()
    {
        OnDestroy();

        for (const auto& component : m_components)
        {
            component->Destroy();
        }
    }

} // namespace GameLibrary