#include "Application.hpp"

#include "Config/TetrisConfig.hpp"
#include "Scene/SceneManager.hpp"
#include "Scenes/SingleGameScene.hpp"
#include "Scenes/TitleScene.hpp"
#include "Services/ConfigService.hpp"
#include "Services/ResourceService.hpp"
#include "Systems/Logger.hpp"

namespace
{
    constexpr auto ENGINE_CONFIG_PATH = "engine.yaml";
    constexpr auto GAME_CONFIG_PATH = "game.yaml";
} // namespace

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

        auto& container = m_engine.GetContainer();
        container.Register<TetrisConfig, TetrisConfig>();

        LoadConfig();
        LoadResources();
        RegisterScenes();

        container.Resolve<GameLibrary::SceneManager>()->LoadScene("Title");
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

        sceneManager->RegisterSceneFactory("SingleGame",
                                           [](const std::string& name, GameLibrary::ServiceContainer& container)
                                           { return std::make_unique<SingleGameScene>(name, container); });
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

    void Application::LoadConfig()
    {
        auto& container = m_engine.GetContainer();
        auto* configService = container.Resolve<GameLibrary::ConfigService>();
        auto* config = container.Resolve<TetrisConfig>();

        if (not configService || not config)
        {
            GameLibrary::Logger::Warning("Failed to load config services");
            return;
        }

        if (not configService->LoadFromFile(GAME_CONFIG_PATH))
        {
            GameLibrary::Logger::Warning("Failed to load game config, using defaults");
            return;
        }

        // Board
        config->boardWidth = configService->GetInt("boardWidth", config->boardWidth);
        config->boardHeight = configService->GetInt("boardHeight", config->boardHeight);
        config->cellSize = configService->GetInt("cellSize", config->cellSize);
        config->boardX = configService->GetInt("boardX", config->boardX);
        config->boardY = configService->GetInt("boardY", config->boardY);

        // Hold Box
        config->holdBoxX = configService->GetInt("holdBoxX", config->holdBoxX);
        config->holdBoxY = configService->GetInt("holdBoxY", config->holdBoxY);
        config->holdBoxSize = configService->GetInt("holdBoxSize", config->holdBoxSize);
        config->holdBoxPadding = configService->GetInt("holdBoxPadding", config->holdBoxPadding);

        // Next Box
        config->nextBoxX = configService->GetInt("nextBoxX", config->nextBoxX);
        config->nextBoxY = configService->GetInt("nextBoxY", config->nextBoxY);
        config->nextBoxSize = configService->GetInt("nextBoxSize", config->nextBoxSize);
        config->nextBoxPadding = configService->GetInt("nextBoxPadding", config->nextBoxPadding);

        // Gameplay
        config->fallInterval = configService->GetFloat("fallInterval", config->fallInterval);
    }
} // namespace Tetris