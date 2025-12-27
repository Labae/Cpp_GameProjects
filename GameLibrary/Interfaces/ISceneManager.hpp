#pragma once

#include <string>

namespace GameLibrary
{
    class Scene;

    class ISceneManager
    {
    public:
        virtual ~ISceneManager() = default;

        virtual void LoadScene(const std::string& name) = 0;
        virtual Scene* GetCurrentScene() const = 0;
    };
}