#include "Scene/SceneManager.hpp"

#include "Interfaces/IGraphics.hpp"
#include "Scene/Scene.hpp"

namespace GameLibrary
{
    void SceneManager::Init(ServiceContainer& container)
    {
        m_container = &container;
    }

    void SceneManager::LoadScene(const std::string& name)
    {
        m_pendingScene = name;
    }

    Scene* SceneManager::GetCurrentScene() const noexcept
    {
        return m_currentScene.get();
    }

    void SceneManager::Update(const float deltaTime)
    {
        if (!m_pendingScene.empty())
        {
            if (m_currentScene)
            {
                m_currentScene->OnExit();
            }

            if (const auto it = m_factories.find(m_pendingScene); it != m_factories.end())
            {
                m_currentScene = it->second();
                m_currentScene->OnEnter();
            }

            m_pendingScene.clear();
        }

        if (m_currentScene)
        {
            m_currentScene->Update(deltaTime);
        }
    }

    void SceneManager::Render(IGraphics& graphics) const
    {
        if (m_currentScene)
        {
            m_currentScene->Render(graphics);
        }
    }
} // namespace GameLibrary