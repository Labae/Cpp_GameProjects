#include "Application.hpp"

#include "Config/SnakeGameConfig.hpp"
#include "Data/GameData.hpp"
#include "Interfaces/ISceneManager.hpp"
#include "Scene/SceneManager.hpp"
#include "Scenes/GameScene.hpp"
#include "Scenes/ResultScene.hpp"
#include "Scenes/TitleScene.hpp"
#include "Systems/Logger.hpp"
#include "Systems/SaveSystem.hpp"
#include "Systems/SoundSystem.hpp"

namespace
{
    constexpr const char* ENGINE_CONFIG_PATH = "engine.yaml";
    constexpr const char* SAVE_FILE_PATH = "savedata.dat";
    constexpr const char* KEY_HIGH_SCORE = "highScore";
    constexpr const char* KEY_BGM_VOLUME = "bgmVolume";
    constexpr const char* KEY_SFX_VOLUME = "sfxVolume";
} // namespace

int32_t Application::Run()
{
    GameLibrary::EngineConfig config{
        .screenWidth = 800,
        .screenHeight = 800,
        .title = "Snake Game",
        .fontPath = "fonts/arial.ttf",
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

    LoadSounds();
    LoadGameData();
    RegisterScenes();

    container.Resolve<GameLibrary::ISceneManager>()->LoadScene("Title");
    m_engine.Run();

    SaveGameData();
    m_engine.Shutdown();

    return 0;
}

void Application::RegisterScenes()
{
    auto* sceneManager = m_engine.GetContainer().Resolve<GameLibrary::ISceneManager>();

    sceneManager->RegisterSceneFactory("Title", [](const std::string& name, GameLibrary::ServiceContainer& container)
                                       { return std::make_unique<TitleScene>(name, container); });
    sceneManager->RegisterSceneFactory("Game", [](const std::string& name, GameLibrary::ServiceContainer& container)
                                       { return std::make_unique<GameScene>(name, container); });
    sceneManager->RegisterSceneFactory("Result", [](const std::string& name, GameLibrary::ServiceContainer& container)
                                       { return std::make_unique<ResultScene>(name, container); });
}

void Application::LoadSounds()
{
    auto* soundSystem = m_engine.GetContainer().Resolve<GameLibrary::SoundSystem>();
    if (soundSystem)
    {
        if (!soundSystem->Load("eat", "assets/sounds/eat.wav"))
        {
            GameLibrary::Logger::Warning("Failed to load sound: eat");
        }
        if (!soundSystem->Load("gameover", "assets/sounds/gameover.wav"))
        {
            GameLibrary::Logger::Warning("Failed to load sound: gameover");
        }
        if (!soundSystem->Load("coin", "assets/sounds/coin.wav"))
        {
            GameLibrary::Logger::Warning("Failed to load sound: coin");
        }
    }
}

void Application::LoadGameData()
{
    auto& container = m_engine.GetContainer();
    auto* saveSystem = container.Resolve<GameLibrary::SaveSystem>();
    auto* soundSystem = container.Resolve<GameLibrary::SoundSystem>();
    auto* gameData = container.Resolve<SnakeGame::GameData>();

    if (saveSystem && gameData)
    {
        if (!saveSystem->LoadFromFile(SAVE_FILE_PATH))
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
    auto* saveSystem = container.Resolve<GameLibrary::SaveSystem>();
    auto* soundSystem = container.Resolve<GameLibrary::SoundSystem>();
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