#include "Application.hpp"

#include "../../GameLibrary/include/Services/AudioService.hpp"
#include "../../GameLibrary/include/Services/SaveService.hpp"
#include "../../GameLibrary/include/Systems/Logger.hpp"
#include "Config/SnakeGameConfig.hpp"
#include "Data/GameData.hpp"
#include "Scene/SceneManager.hpp"
#include "Scenes/GameScene.hpp"
#include "Scenes/ResultScene.hpp"
#include "Scenes/TitleScene.hpp"

namespace
{
    constexpr auto ENGINE_CONFIG_PATH = "engine.yaml";
    constexpr auto SAVE_FILE_PATH = "savedata.dat";
    constexpr auto KEY_HIGH_SCORE = "highScore";
    constexpr auto KEY_BGM_VOLUME = "bgmVolume";
    constexpr auto KEY_SFX_VOLUME = "sfxVolume";
} // namespace

int32_t Application::Run()
{
    const GameLibrary::EngineConfig config{
        .screenWidth = 800,
        .screenHeight = 800,
        .title = "Snake Game",
        .configPath = ENGINE_CONFIG_PATH,
    };

    if (!m_engine.Init(config))
    {
        return -1;
    }

    auto& container = m_engine.GetContainer();

    // 게임 전용 데이터 등록
    container.Register<SnakeGame::SnakeGameConfig, SnakeGame::SnakeGameConfig>();
    container.Register<SnakeGame::GameData, SnakeGame::GameData>();

    LoadResources();
    LoadGameData();
    RegisterScenes();

    container.Resolve<GameLibrary::SceneManager>()->LoadScene("Title");
    m_engine.Run();

    SaveGameData();
    m_engine.Shutdown();

    return 0;
}

void Application::RegisterScenes()
{
    auto* sceneManager = m_engine.GetContainer().Resolve<GameLibrary::SceneManager>();

    sceneManager->RegisterSceneFactory("Title", [](const std::string& name, GameLibrary::ServiceContainer& container)
                                       { return std::make_unique<TitleScene>(name, container); });
    sceneManager->RegisterSceneFactory("Game", [](const std::string& name, GameLibrary::ServiceContainer& container)
                                       { return std::make_unique<GameScene>(name, container); });
    sceneManager->RegisterSceneFactory("Result", [](const std::string& name, GameLibrary::ServiceContainer& container)
                                       { return std::make_unique<ResultScene>(name, container); });
}

void Application::LoadResources()
{
    auto* resourceService = m_engine.GetContainer().Resolve<GameLibrary::ResourceService>();
    if (not resourceService)
    {
        GameLibrary::Logger::Error("Resource service not found");
        return;
    }

    // 사운드 로드
    if (not resourceService->LoadSound("eat", "assets/sounds/eat.wav"))
    {
        GameLibrary::Logger::Warning("Failed to load sound: eat");
    }
    if (not resourceService->LoadSound("gameover", "assets/sounds/gameover.wav"))
    {
        GameLibrary::Logger::Warning("Failed to load sound: gameover");
    }
    if (not resourceService->LoadSound("coin", "assets/sounds/coin.wav"))
    {
        GameLibrary::Logger::Warning("Failed to load sound: coin");
    }

    // 폰트 로드
    if (not resourceService->LoadFont("main", "assets/fonts/NotoSans_Black.ttf"))
    {
        GameLibrary::Logger::Warning("Failed to load font: main");
    }
}

void Application::LoadGameData()
{
    auto& container = m_engine.GetContainer();
    auto* saveSystem = container.Resolve<GameLibrary::SaveService>();
    auto* soundSystem = container.Resolve<GameLibrary::AudioService>();
    auto* gameData = container.Resolve<SnakeGame::GameData>();

    if (saveSystem and gameData)
    {
        if (not saveSystem->LoadFromFile(SAVE_FILE_PATH))
        {
            GameLibrary::Logger::Info("No save file found, using defaults");
        }
        gameData->highScore = saveSystem->GetInt(KEY_HIGH_SCORE, 0);
        gameData->bgmVolume = saveSystem->GetFloat(KEY_BGM_VOLUME, 1.0f);
        gameData->sfxVolume = saveSystem->GetFloat(KEY_SFX_VOLUME, 1.0f);

        if (soundSystem)
        {
            soundSystem->SetBGMVolume(gameData->bgmVolume);
            soundSystem->SetSFXVolume(gameData->sfxVolume);
        }
    }
}

void Application::SaveGameData()
{
    auto& container = m_engine.GetContainer();
    auto* saveSystem = container.Resolve<GameLibrary::SaveService>();
    auto* soundSystem = container.Resolve<GameLibrary::AudioService>();
    auto* gameData = container.Resolve<SnakeGame::GameData>();

    if (saveSystem && gameData)
    {
        if (soundSystem)
        {
            gameData->bgmVolume = soundSystem->GetBGMVolume();
            gameData->sfxVolume = soundSystem->GetSFXVolume();
        }

        saveSystem->SetInt(KEY_HIGH_SCORE, gameData->highScore);
        saveSystem->SetFloat(KEY_BGM_VOLUME, gameData->bgmVolume);
        saveSystem->SetFloat(KEY_SFX_VOLUME, gameData->sfxVolume);

        if (!saveSystem->SaveToFile(SAVE_FILE_PATH))
        {
            GameLibrary::Logger::Error("Failed to save game data");
        }
    }
}