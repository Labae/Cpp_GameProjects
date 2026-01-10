#pragma once

#include "Interfaces/IInitializable.hpp"
#include "Scene.hpp"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace GameLibrary
{
    class SceneManager : public IInitializable
    {
    public:
        SceneManager() = default;
        ~SceneManager() override = default;

        SceneManager(const SceneManager&) = delete;
        SceneManager(SceneManager&&) = default;
        SceneManager& operator=(const SceneManager&) = delete;
        SceneManager& operator=(SceneManager&&) = default;

        void Init(ServiceContainer& container) override;

        void RegisterSceneFactory(
            const std::string& name,
            const std::function<std::unique_ptr<Scene>(const std::string&, ServiceContainer&)>& factory)
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

        void LoadScene(const std::string& name);
        [[nodiscard]] Scene* GetCurrentScene() const noexcept;

        void Update(float deltaTime);
        void Render(IGraphics& graphics) const;

    private:
        std::unordered_map<std::string, std::function<std::unique_ptr<Scene>()>> m_factories{};
        std::unique_ptr<Scene> m_currentScene{};
        std::string m_pendingScene{};
        ServiceContainer* m_container{};
    };
} // namespace GameLibrary