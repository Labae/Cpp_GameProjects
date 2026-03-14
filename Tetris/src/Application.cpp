#include "Application.hpp"

#include "Config/TetrisConfig.hpp"
#include "Data/GameData.hpp"
#include "Events/GameEvents.hpp"
#include "Scene/SceneManager.hpp"
#include "Scenes/SingleGameScene.hpp"
#include "Scenes/TitleScene.hpp"
#include "Services/ConfigService.hpp"
#include "Services/ResourceService.hpp"
#include "Services/SaveService.hpp"
#include "Systems/Logger.hpp"

namespace
{
    constexpr auto ENGINE_CONFIG_PATH = "engine.yaml";
    constexpr auto GAME_CONFIG_PATH = "game.yaml";
    constexpr auto SAVE_FILE_PATH = "tetris_save.dat";
    constexpr auto KEY_HIGH_SCORE = "highScore";
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
        container.Register<GameData, GameData>();

        LoadConfig();
        LoadGameData();
        LoadResources();
        RegisterScenes();
        SubscribeEvents();

        container.Resolve<GameLibrary::SceneManager>()->LoadScene("Title");
        m_engine.Run();

        SaveGameData();
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

        // Scores
        config->lineScores[0] = configService->GetInt("scoreSingle", config->lineScores[0]);
        config->lineScores[1] = configService->GetInt("scoreDouble", config->lineScores[1]);
        config->lineScores[2] = configService->GetInt("scoreTriple", config->lineScores[2]);
        config->lineScores[3] = configService->GetInt("scoreTetris", config->lineScores[3]);
    }

    void Application::LoadGameData()
    {
        auto& container = m_engine.GetContainer();
        auto* saveService = container.Resolve<GameLibrary::SaveService>();
        auto* gameData = container.Resolve<GameData>();

        if (not saveService || not gameData)
        {
            return;
        }

        if (not saveService->LoadFromFile(SAVE_FILE_PATH))
        {
            GameLibrary::Logger::Info("No save file found, using defaults");
        }

        gameData->highScore = saveService->GetInt(KEY_HIGH_SCORE, 0);
    }

    void Application::SaveGameData()
    {
        auto& container = m_engine.GetContainer();
        auto* saveService = container.Resolve<GameLibrary::SaveService>();
        auto* gameData = container.Resolve<GameData>();

        if (not saveService || not gameData)
        {
            return;
        }

        saveService->SetInt(KEY_HIGH_SCORE, gameData->highScore);

        if (not saveService->SaveToFile(SAVE_FILE_PATH))
        {
            GameLibrary::Logger::Error("Failed to save game data");
        }
    }
} // namespace Tetris