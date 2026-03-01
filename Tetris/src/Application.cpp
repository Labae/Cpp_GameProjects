//
// Created by Labae on 1/12/26.
//

#include "Application.hpp"

#include "Configs/TetrisConfig.hpp"
#include "Constants/Resources.hpp"
#include "Constants/SceneNames.hpp"
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

        LoadConfigs();
        LoadResources();
        RegisterScenes();

        m_engine.GetContainer().Resolve<GameLibrary::SceneManager>()->LoadScene(SceneNames::Title);
        m_engine.Run();
        m_engine.Shutdown();

        return 0;
    }

    void Application::RegisterScenes()
    {
        auto* sceneManager = m_engine.GetContainer().Resolve<GameLibrary::SceneManager>();

        sceneManager->RegisterSceneFactory(SceneNames::Title,
                                           [](const std::string& name, GameLibrary::ServiceContainer& container)
                                           { return std::make_unique<TitleScene>(name, container); });

        sceneManager->RegisterSceneFactory(SceneNames::SingleGame,
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

        if (not resourceService->LoadFont(Resources::Fonts::Main.Id, Resources::Fonts::Main.Path))
        {
            GameLibrary::Logger::Error("Failed to load main font");
        }
    }

    void Application::LoadConfigs()
    {
        auto& container = m_engine.GetContainer();
        auto* configService = container.Resolve<GameLibrary::ConfigService>();
        auto* tetrisConfig = container.Resolve<TetrisConfig>();

        if (not configService || not tetrisConfig)
        {
            GameLibrary::Logger::Error("Failed to load config services");
            return;
        }

        if (not configService->LoadFromFile(GAME_CONFIG_PATH))
        {
            GameLibrary::Logger::Error("Failed to load game config, using defaults");
            return;
        }

        tetrisConfig->boardWidth = configService->GetInt("boardWidth", tetrisConfig->boardWidth);
        tetrisConfig->boardHeight = configService->GetInt("boardHeight", tetrisConfig->boardHeight);
        tetrisConfig->cellSize = configService->GetInt("cellSize", tetrisConfig->cellSize);
        tetrisConfig->boardX = configService->GetInt("boardX", tetrisConfig->boardX);
        tetrisConfig->boardY = configService->GetInt("boardY", tetrisConfig->boardY);
        tetrisConfig->holdNextBoxSize = configService->GetInt("holdNextBoxSize", tetrisConfig->holdNextBoxSize);
    }
} // namespace Tetris
