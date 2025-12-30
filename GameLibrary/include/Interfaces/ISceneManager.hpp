#pragma once

#include <functional>
#include <memory>
#include <string>

namespace GameLibrary
{
    class Scene;
    class IGraphics;
    class ServiceContainer;

    class ISceneManager
    {
    public:
        virtual ~ISceneManager() = default;

        virtual void Update(float deltaTime) = 0;
        virtual void Render(IGraphics& graphics) = 0;

        virtual void LoadScene(const std::string& name) = 0;
        [[nodiscard]] virtual Scene* GetCurrentScene() const = 0;

        // 팩토리 등록
        virtual void RegisterSceneFactory(
            const std::string& name,
            std::function<std::unique_ptr<Scene>(const std::string&, ServiceContainer&)> factory) = 0;
    };
} // namespace GameLibrary