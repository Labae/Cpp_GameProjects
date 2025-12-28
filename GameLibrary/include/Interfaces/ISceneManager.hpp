#pragma once

#include <string>

namespace GameLibrary
{
    class Scene;
    class IGraphics;

    class ISceneManager
    {
    public:
        virtual ~ISceneManager() = default;

        virtual void Update(float deltaTime) = 0;
        virtual void Render(IGraphics& graphics) = 0;

        virtual void LoadScene(const std::string& name) = 0;
        [[nodiscard]] virtual Scene* GetCurrentScene() const = 0;
    };
} // namespace GameLibrary