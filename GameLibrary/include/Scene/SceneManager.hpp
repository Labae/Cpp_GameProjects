#pragma once

#include "Interfaces/IInitializable.hpp"
#include "Interfaces/ISceneManager.hpp"
#include "Scene.hpp"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace GameLibrary
{
    class SceneManager : public ISceneManager, public IInitializable
    {
    public:
        SceneManager() = default;
        ~SceneManager() = default;

        SceneManager(const SceneManager&) = delete;
        SceneManager(SceneManager&&) = default;
        SceneManager& operator=(const SceneManager&) = delete;
        SceneManager& operator=(SceneManager&&) = default;

        void Init(ServiceContainer& container) override;

        void RegisterSceneFactory(
            const std::string& name,
            std::function<std::unique_ptr<Scene>(const std::string&, ServiceContainer&)> factory) override
        {
            m_factories[name] = [this, factory, name]() -> std::unique_ptr<Scene>
            {
                return factory(name, *m_container);
            };
        }

        template <typename T> void RegisterScene(const std::string& name)
        {
            m_factories[name] = [this, name]() -> std::unique_ptr<Scene>
            {
                return std::make_unique<T>(name, *m_container);
            };
        }

        void LoadScene(const std::string& name) override;
        [[nodiscard]] Scene* GetCurrentScene() const noexcept override;

        void Update(float deltaTime) override;
        void Render(IGraphics& graphics) override;

    private:
        std::unordered_map<std::string, std::function<std::unique_ptr<Scene>()>> m_factories{};
        std::unique_ptr<Scene> m_currentScene{};
        std::string m_pendingScene{};
        ServiceContainer* m_container{};
    };
} // namespace GameLibrary