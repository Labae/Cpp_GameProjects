#include "Scene/Scene.hpp"

#include "GameObject/GameObject.hpp"
#include "Interfaces/IGraphics.hpp"

#include <algorithm>

namespace GameLibrary
{
    Scene::Scene(const std::string& name, ServiceContainer& container) noexcept : m_name(name), m_container(container)
    {
    }

    GameObject* Scene::CreateGameObject()
    {
        auto gameObject = std::make_unique<GameObject>();
        auto* ptr = gameObject.get();
        m_gameObjects.emplace_back(std::move(gameObject));
        return ptr;
    }

    void Scene::DestroyGameObject(GameObject* gameObject)
    {
        m_pendingDestroy.emplace_back(gameObject);
    }

    void Scene::Update(float deltaTime)
    {
        for (auto& gameObject : m_gameObjects)
        {
            if (gameObject->IsActive())
            {
                gameObject->Update(deltaTime);
            }
        }

        for (auto* gameObject : m_pendingDestroy)
        {
            gameObject->Destroy();
            auto it = std::find_if(m_gameObjects.begin(), m_gameObjects.end(),
                                   [gameObject](const auto& ptr) { return ptr.get() == gameObject; });
            if (it != m_gameObjects.end())
            {
                m_gameObjects.erase(it);
            }
        }
        m_pendingDestroy.clear();
    }

    void Scene::Render(IGraphics& graphics)
    {
        for (auto& gameObject : m_gameObjects)
        {
            if (gameObject->IsActive())
            {
                gameObject->Render(graphics);
            }
        }
    }

    void Scene::Clear()
    {
        for (auto& gameObject : m_gameObjects)
        {
            gameObject->Destroy();
        }
        m_gameObjects.clear();
        m_pendingDestroy.clear();
    }
} // namespace GameLibrary