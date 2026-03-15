#include "Application.hpp"

#include "Events/GameEvents.hpp"
#include "Scene/SceneManager.hpp"
#include "Scenes/DungeonScene.hpp"
#include "Scenes/JobSelectScene.hpp"
#include "Scenes/TitleScene.hpp"
#include "Services/ResourceService.hpp"
#include "Systems/Logger.hpp"

namespace
{
    constexpr auto ENGINE_CONFIG_PATH = "engine.yaml";
} // namespace

namespace TextRPG
{
    int32_t Application::Run()
    {
        const GameLibrary::EngineConfig config{
            .screenWidth = 800,
            .screenHeight = 600,
            .title = "Dungeon Descent",
            .configPath = ENGINE_CONFIG_PATH,
        };

        if (not m_engine.Init(config))
        {
            return -1;
        }

        LoadResources();
        RegisterScenes();
        SubscribeEvents();


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

        sceneManager->RegisterSceneFactory("JobSelect",
                                   [](const std::string& name, GameLibrary::ServiceContainer& container)
                                   { return std::make_unique<JobSelectScene>(name, container); });

        sceneManager->RegisterSceneFactory("Dungeon",
                           [](const std::string& name, GameLibrary::ServiceContainer& container)
                           { return std::make_unique<DungeonScene>(name, container); });
    }

    void Application::SubscribeEvents()
    {
        if (auto* eventService = m_engine.GetContainer().Resolve<GameLibrary::EventService>())
        {
            m_quitToken = eventService->Subscribe<QuitEvent>([this]([[maybe_unused]] const QuitEvent& event)
            {
                m_engine.Quit();
            });
        }
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