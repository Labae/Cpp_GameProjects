#include "Application.hpp"
#include "Scene/SceneManager.hpp"
#include "Scenes/TitleScene.hpp"
#include "Scenes/GameScene.hpp"
#include "Scenes/ResultScene.hpp"
#include "Config/SnakeGameConfig.hpp"
#include "Data/GameData.hpp"
#include "Systems/SaveSystem.hpp"
#include "Systems/SoundSystem.hpp"
#include "Systems/Logger.hpp"

namespace
{
    constexpr const char* ENGINE_CONFIG_PATH = "engine.yaml";
    constexpr const char* SAVE_FILE_PATH = "savedata.dat";
    constexpr const char* KEY_HIGH_SCORE = "highScore";
    constexpr const char* KEY_BGM_VOLUME = "bgmVolume";
    constexpr const char* KEY_SFX_VOLUME = "sfxVolume";
}

int32_t Application::Run(HINSTANCE hInstance)
{
    GameLibrary::Logger::Init();

    if (!m_engine.Init(hInstance, L"Snake Game", ENGINE_CONFIG_PATH))
    {
        return -1;
    }

    // 게임 전용 데이터 등록
    m_engine.GetContainer().Register<SnakeGame::SnakeGameConfig, SnakeGame::SnakeGameConfig>();
    m_engine.GetContainer().Register<SnakeGame::GameData, SnakeGame::GameData>();

    // 사운드 로드
    LoadSounds();

    // 저장된 데이터 로드
    LoadGameData();

    RegisterScenes();
    m_engine.GetSceneManager()->LoadScene("Title");

    m_engine.Run();

    // 종료 시 저장
    SaveGameData();

    m_engine.Shutdown();

    GameLibrary::Logger::Shutdown();

    return 0;
}

void Application::RegisterScenes()
{
    m_engine.GetSceneManager()->RegisterScene<TitleScene>("Title");
    m_engine.GetSceneManager()->RegisterScene<GameScene>("Game");
    m_engine.GetSceneManager()->RegisterScene<ResultScene>("Result");
}

void Application::LoadSounds()
{
    auto* soundSystem = m_engine.GetSoundSystem();
    if (soundSystem)
    {
        soundSystem->Load("eat", L"sounds/eat.wav");
        soundSystem->Load("gameover", L"sounds/gameover.wav");
        soundSystem->Load("coin", L"sounds/coin.wav");
    }
}

void Application::LoadGameData()
{
    auto* saveSystem = m_engine.GetSaveSystem();
    auto* soundSystem = m_engine.GetSoundSystem();
    auto* gameData = m_engine.GetContainer().Resolve<SnakeGame::GameData>();

    if (saveSystem && gameData)
    {
        if (!saveSystem->LoadFromFile(SAVE_FILE_PATH))
        {
            GameLibrary::Logger::Info("No save file found, using defaults");
        }
        gameData->highScore = saveSystem->GetInt(KEY_HIGH_SCORE, 0);
        gameData->bgmVolume = saveSystem->GetFloat(KEY_BGM_VOLUME, 1.0f);
        gameData->sfxVolume = saveSystem->GetFloat(KEY_SFX_VOLUME, 1.0f);

        // SoundSystem에 볼륨 적용
        if (soundSystem)
        {
            soundSystem->SetBGMVolume(gameData->bgmVolume);
            soundSystem->SetSFXVolume(gameData->sfxVolume);
        }
    }
}

void Application::SaveGameData()
{
    auto* saveSystem = m_engine.GetSaveSystem();
    auto* soundSystem = m_engine.GetSoundSystem();
    auto* gameData = m_engine.GetContainer().Resolve<SnakeGame::GameData>();

    if (saveSystem && gameData)
    {
        // SoundSystem에서 현재 볼륨 가져오기
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