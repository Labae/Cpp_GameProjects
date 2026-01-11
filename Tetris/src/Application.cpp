//
// Created by Labae on 1/12/26.
//

#include "Application.hpp"

#include "Scene/SceneManager.hpp"
#include "Scenes/TitleScene.hpp"
#include "Services/ResourceService.hpp"
#include "Systems/Logger.hpp"

namespace
{
    constexpr auto ENGINE_CONFIG_PATH = "engine.yaml";
}

namespace Tetris
{
    int32_t Application::Run()
    {
        const GameLibrary::EngineConfig config{
            .screenWidth = 800,
            .screenHeight = 800,
            .title = "Tetris",
            .configPath = ENGINE_CONFIG_PATH,
        };

        if (not m_engine.Init(config))
        {
            return -1;
        }

        LoadResources();
        RegisterScenes();

        m_engine.GetContainer().Resolve<GameLibrary::SceneManager>()->LoadScene("Title");
        m_engine.Run();
        m_engine.Shutdown();

        return 0;
    }

    void Application::RegisterScenes()
    {
        auto* sceneManager = m_engine.GetContainer().Resolve<GameLibrary::SceneManager>();

        sceneManager->RegisterSceneFactory("Title",
                                           [](const std::string& name, GameLibrary::ServiceContainer& container)
                                           { return std::make_unique<TitleScene>(name, container); });
    }

    void Application::LoadResources()
    {
        auto* resourceService = m_engine.GetContainer().Resolve<GameLibrary::ResourceService>();
        if (not resourceService)
        {
            GameLibrary::Logger::Error("Resource service not found");
            return;
        }

        if (not resourceService->LoadFont("main", "assets/fonts/NotoSans_Black.ttf"))
        {
            GameLibrary::Logger::Error("Failed to load main font");
        }
    }
} // namespace Tetris
