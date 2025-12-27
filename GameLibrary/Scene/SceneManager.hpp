#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>
#include "Interfaces/ISceneManager.hpp"
#include "Scene.hpp"

namespace GameLibrary
{
    class SceneManager : public ISceneManager
    {
    public:
        SceneManager() = default;
        ~SceneManager() = default;

        template<typename T>
        void RegisterScene(const std::string& name)
        {
            m_factories[name] = [this, name]() -> std::unique_ptr<Scene>
                {
                    return std::make_unique<T>(name, *m_container);
                };
        }

        void LoadScene(const std::string& name) override;
        [[nodiscard]] Scene* GetCurrentScene() const noexcept override;

        void SetContainer(ServiceContainer* container) noexcept { m_container = container; }

        void Update(float deltaTime);
        void Render(IGraphics& graphics);

    private:
        std::unordered_map<std::string, std::function<std::unique_ptr<Scene>()>> m_factories;
        std::unique_ptr<Scene> m_currentScene;
        std::string m_pendingScene;
        ServiceContainer* m_container = nullptr;
    };
}