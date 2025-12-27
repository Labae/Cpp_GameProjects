#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Interfaces/IGraphics.hpp"

namespace GameLibrary
{
    void SceneManager::LoadScene(const std::string& name)
    {
        m_pendingScene = name;
    }

    Scene* SceneManager::GetCurrentScene() const noexcept
    {
        return m_currentScene.get();
    }

    void SceneManager::Update(float deltaTime)
    {
        if (!m_pendingScene.empty())
        {
            if (m_currentScene)
            {
                m_currentScene->OnExit();
            }

            auto it = m_factories.find(m_pendingScene);
            if (it != m_factories.end())
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

    void SceneManager::Render(IGraphics& graphics)
    {
        if (m_currentScene)
        {
            m_currentScene->Render(graphics);
        }
    }
}