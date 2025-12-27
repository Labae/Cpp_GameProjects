#include "GameScene.hpp"
#include "GameObject/GameObject.hpp"
#include "Core/ServiceContainer.hpp"
#include "Core/EngineConfig.hpp"
#include "Config/SnakeGameConfig.hpp"
#include "Data/GameData.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Interfaces/ISceneManager.hpp"
#include "Systems/EventSystem.hpp"
#include "Systems/ConfigSystem.hpp"
#include "Systems/SoundSystem.hpp"
#include "Systems/SaveSystem.hpp"
#include "Systems/Logger.hpp"
#include "Systems/FxSystem.hpp"
#include "Components/SnakeController.hpp"
#include "Components/Pickup.hpp"
#include "Components/FoodSpawner.hpp"
#include "Components/GameManager.hpp"
#include "Components/GridBackground.hpp"

namespace
{
    constexpr const char* GAME_CONFIG_PATH = "game.yaml";
}

GameScene::GameScene(const std::string& name, GameLibrary::ServiceContainer& container)
    : Scene(name, container)
{
}

void GameScene::OnEnter()
{
    m_fxSystem = std::make_unique<GameLibrary::FxSystem>();
    m_pauseState = PauseState::None;
    m_menuIndex = 0;
    m_settingsIndex = 0;

    auto& container = GetContainer();
    m_input = container.Resolve<GameLibrary::IInputProvider>();
    m_engineConfig = container.Resolve<GameLibrary::EngineConfig>();
    m_soundSystem = container.Resolve<GameLibrary::SoundSystem>();
    m_saveSystem = container.Resolve<GameLibrary::SaveSystem>();
    m_gameData = container.Resolve<SnakeGame::GameData>();

    // 저장된 볼륨 적용
    if (m_soundSystem && m_gameData)
    {
        m_soundSystem->SetBGMVolume(m_gameData->bgmVolume);
        m_soundSystem->SetSFXVolume(m_gameData->sfxVolume);
    }

    ApplyConfig();
    SetupGame();

    // 배경 음악 재생
    if (m_soundSystem)
    {
        m_soundSystem->PlayBGM(L"sounds/background.wav");
    }
}

void GameScene::OnExit()
{
    // 배경 음악 중지
    if (m_soundSystem)
    {
        m_soundSystem->StopBGM();
    }

    Clear();
    m_fxSystem.reset();
}

void GameScene::Update(float deltaTime)
{
    if (!m_input) return;

    switch (m_pauseState)
    {
    case PauseState::None:
        if (m_input->IsKeyPressed(GameLibrary::KeyCode::Escape))
        {
            m_pauseState = PauseState::Menu;
            m_menuIndex = 0;
        }
        else
        {
            Scene::Update(deltaTime);
            if (m_fxSystem)
            {
                m_fxSystem->Update(deltaTime);
            }
        }
        break;

    case PauseState::Menu:
        UpdatePauseMenu();
        break;

    case PauseState::Settings:
        UpdateSettings();
        break;
    }
}

void GameScene::UpdatePauseMenu()
{
    if (m_input->IsKeyPressed(GameLibrary::KeyCode::Up))
    {
        m_menuIndex = (m_menuIndex + 2) % 3; // 0->2, 1->0, 2->1
    }
    if (m_input->IsKeyPressed(GameLibrary::KeyCode::Down))
    {
        m_menuIndex = (m_menuIndex + 1) % 3; // 0->1, 1->2, 2->0
    }
    if (m_input->IsKeyPressed(GameLibrary::KeyCode::Enter) || m_input->IsKeyPressed(GameLibrary::KeyCode::Space))
    {
        switch (m_menuIndex)
        {
        case 0: // Resume
            m_pauseState = PauseState::None;
            break;
        case 1: // Settings
            m_pauseState = PauseState::Settings;
            m_settingsIndex = 0;
            break;
        case 2: // Quit
            if (auto* sceneManager = GetContainer().Resolve<GameLibrary::ISceneManager>())
            {
                sceneManager->LoadScene("Title");
            }
            break;
        }
    }
    if (m_input->IsKeyPressed(GameLibrary::KeyCode::Escape))
    {
        m_pauseState = PauseState::None;
    }
}

void GameScene::UpdateSettings()
{
    if (m_input->IsKeyPressed(GameLibrary::KeyCode::Up))
    {
        m_settingsIndex = (m_settingsIndex + 1) % 2;
    }
    if (m_input->IsKeyPressed(GameLibrary::KeyCode::Down))
    {
        m_settingsIndex = (m_settingsIndex + 1) % 2;
    }

    float delta = 0.1f;
    bool changed = false;

    if (m_input->IsKeyPressed(GameLibrary::KeyCode::Left))
    {
        if (m_soundSystem)
        {
            if (m_settingsIndex == 0)
            {
                m_soundSystem->SetBGMVolume(m_soundSystem->GetBGMVolume() - delta);
            }
            else
            {
                m_soundSystem->SetSFXVolume(m_soundSystem->GetSFXVolume() - delta);
            }
            changed = true;
        }
    }
    if (m_input->IsKeyPressed(GameLibrary::KeyCode::Right))
    {
        if (m_soundSystem)
        {
            if (m_settingsIndex == 0)
            {
                m_soundSystem->SetBGMVolume(m_soundSystem->GetBGMVolume() + delta);
            }
            else
            {
                m_soundSystem->SetSFXVolume(m_soundSystem->GetSFXVolume() + delta);
            }
            changed = true;
        }
    }

    if (changed)
    {
        SaveSettings();
    }

    if (m_input->IsKeyPressed(GameLibrary::KeyCode::Escape))
    {
        m_pauseState = PauseState::Menu;
    }
}

void GameScene::SaveSettings()
{
    if (!m_gameData || !m_soundSystem || !m_saveSystem) return;

    m_gameData->bgmVolume = m_soundSystem->GetBGMVolume();
    m_gameData->sfxVolume = m_soundSystem->GetSFXVolume();

    m_saveSystem->SetInt("highScore", m_gameData->highScore);
    m_saveSystem->SetFloat("bgmVolume", m_gameData->bgmVolume);
    m_saveSystem->SetFloat("sfxVolume", m_gameData->sfxVolume);
    if (!m_saveSystem->SaveToFile("savedata.dat"))
    {
        GameLibrary::Logger::Warning("Failed to save GameData!!");
    }
}

void GameScene::Render(GameLibrary::IGraphics& graphics)
{
    if (m_fxSystem)
    {
        auto [sx, sy] = m_fxSystem->GetShakeOffset();
        graphics.SetOffset(sx, sy);
    }

    Scene::Render(graphics);

    graphics.ResetOffset();

    if (m_fxSystem)
    {
        m_fxSystem->Render(graphics);
    }

    switch (m_pauseState)
    {
    case PauseState::Menu:
        RenderPauseMenu(graphics);
        break;
    case PauseState::Settings:
        RenderSettings(graphics);
        break;
    default:
        break;
    }
}

void GameScene::RenderPauseMenu(GameLibrary::IGraphics& graphics)
{
    if (!m_engineConfig) return;

    int screenW = m_engineConfig->screenWidth;
    int screenH = m_engineConfig->screenHeight;

    // 반투명 오버레이
    graphics.FillRect(0, 0, screenW, screenH, { 0, 0, 0, 150 });

    // 일시정지 박스
    int boxW = 300;
    int boxH = 200;
    int boxX = (screenW - boxW) / 2;
    int boxY = (screenH - boxH) / 2;

    graphics.FillRect(boxX, boxY, boxW, boxH, { 40, 40, 40, 255 });
    graphics.DrawRect(boxX, boxY, boxW, boxH, { 255, 255, 255, 255 });

    // 타이틀
    graphics.DrawLabel(L"PAUSED", boxX, boxY + 20, boxW, 36, { 255, 255, 255, 255 }, GameLibrary::TextAlign::Center);

    // 메뉴 항목
    const wchar_t* items[] = { L"Resume", L"Settings", L"Quit" };
    for (int i = 0; i < 3; ++i)
    {
        GameLibrary::Color color = (i == m_menuIndex) ? GameLibrary::Color{ 255, 255, 0, 255 } : GameLibrary::Color{ 180, 180, 180, 255 };
        graphics.DrawLabel(items[i], boxX, boxY + 70 + i * 35, boxW, 24, color, GameLibrary::TextAlign::Center);
    }
}

void GameScene::RenderSettings(GameLibrary::IGraphics& graphics)
{
    if (!m_engineConfig) return;

    int screenW = m_engineConfig->screenWidth;
    int screenH = m_engineConfig->screenHeight;

    // 반투명 오버레이
    graphics.FillRect(0, 0, screenW, screenH, { 0, 0, 0, 150 });

    // 설정 박스
    int boxW = 350;
    int boxH = 200;
    int boxX = (screenW - boxW) / 2;
    int boxY = (screenH - boxH) / 2;

    graphics.FillRect(boxX, boxY, boxW, boxH, { 40, 40, 40, 255 });
    graphics.DrawRect(boxX, boxY, boxW, boxH, { 255, 255, 255, 255 });

    // 타이틀
    graphics.DrawLabel(L"SETTINGS", boxX, boxY + 20, boxW, 36, { 255, 255, 255, 255 }, GameLibrary::TextAlign::Center);

    // 볼륨 바 그리기
    float bgmVol = m_soundSystem ? m_soundSystem->GetBGMVolume() : 1.0f;
    float sfxVol = m_soundSystem ? m_soundSystem->GetSFXVolume() : 1.0f;

    int barX = boxX + 150;
    int barW = 150;
    int barH = 20;

    // BGM
    GameLibrary::Color bgmColor = (m_settingsIndex == 0) ? GameLibrary::Color{ 255, 255, 0, 255 } : GameLibrary::Color{ 180, 180, 180, 255 };
    graphics.DrawLabel(L"BGM", boxX + 20, boxY + 70, 100, 24, bgmColor, GameLibrary::TextAlign::Left);
    graphics.DrawRect(barX, boxY + 70, barW, barH, { 100, 100, 100, 255 });
    graphics.FillRect(barX, boxY + 70, static_cast<int>(barW * bgmVol), barH, { 0, 200, 0, 255 });

    // SFX
    GameLibrary::Color sfxColor = (m_settingsIndex == 1) ? GameLibrary::Color{ 255, 255, 0, 255 } : GameLibrary::Color{ 180, 180, 180, 255 };
    graphics.DrawLabel(L"SFX", boxX + 20, boxY + 110, 100, 24, sfxColor, GameLibrary::TextAlign::Left);
    graphics.DrawRect(barX, boxY + 110, barW, barH, { 100, 100, 100, 255 });
    graphics.FillRect(barX, boxY + 110, static_cast<int>(barW * sfxVol), barH, { 0, 200, 0, 255 });

    // 안내
    graphics.DrawLabel(L"< > to adjust, ESC to back", boxX, boxY + 160, boxW, 18, { 120, 120, 120, 255 }, GameLibrary::TextAlign::Center);
}

void GameScene::ApplyConfig()
{
    auto& container = GetContainer();
    auto* configSystem = container.Resolve<GameLibrary::ConfigSystem>();
    auto* gameConfig = container.Resolve<SnakeGame::SnakeGameConfig>();

    if (configSystem && gameConfig)
    {
        if (!configSystem->LoadFromFile(GAME_CONFIG_PATH))
        {
            GameLibrary::Logger::Warning("Failed to load game config: " + std::string(GAME_CONFIG_PATH));
        }
        gameConfig->gridSize = configSystem->GetInt("gridSize", gameConfig->gridSize);
        gameConfig->moveInterval = configSystem->GetFloat("moveInterval", gameConfig->moveInterval);
        gameConfig->gameOverDelay = configSystem->GetFloat("gameOverDelay", gameConfig->gameOverDelay);
        gameConfig->headColor = configSystem->GetColor("headColor", gameConfig->headColor);
        gameConfig->tailColor = configSystem->GetColor("tailColor", gameConfig->tailColor);
    }
}

void GameScene::SetupGame()
{
    auto& container = GetContainer();
    auto* engineConfig = container.Resolve<GameLibrary::EngineConfig>();
    auto* gameConfig = container.Resolve<SnakeGame::SnakeGameConfig>();
    auto* gameData = container.Resolve<SnakeGame::GameData>();
    auto* input = container.Resolve<GameLibrary::IInputProvider>();
    auto* sceneManager = container.Resolve<GameLibrary::ISceneManager>();
    auto* eventSystem = container.Resolve<GameLibrary::EventSystem>();
    auto* soundSystem = container.Resolve<GameLibrary::SoundSystem>();

    // GridBackground (가장 먼저 - 배경에 그려짐)
    auto* grid = CreateGameObject();
    grid->AddComponent<GridBackground>(engineConfig->screenWidth, engineConfig->screenHeight, gameConfig->gridSize);

    // GameManager
    auto* manager = CreateGameObject();
    manager->AddComponent<GameManager>(*eventSystem, *input, *sceneManager, *soundSystem, *engineConfig, *gameConfig, *gameData, *m_fxSystem);

    // Snake
    auto* snake = CreateGameObject();
    snake->GetTransform().position = {
        static_cast<float>((engineConfig->screenWidth / 2 / gameConfig->gridSize) * gameConfig->gridSize),
        static_cast<float>((engineConfig->screenHeight / 2 / gameConfig->gridSize) * gameConfig->gridSize)
    };
    snake->AddComponent<SnakeController>(snake->GetTransform(), *input, *eventSystem, *engineConfig, *gameConfig);

    // Food
    auto* food = CreateGameObject();
    food->AddComponent<Pickup>(food->GetTransform(), *eventSystem, gameConfig->gridSize);

    auto* goldenFood = CreateGameObject();
    auto* goldenPickup = goldenFood->AddComponent<GoldenPickup>(goldenFood->GetTransform(), *eventSystem, gameConfig->gridSize);

    // FoodSpawner
    auto* spawner = CreateGameObject();
    spawner->AddComponent<FoodSpawner>(food->GetTransform(), *goldenPickup, *eventSystem, *engineConfig, *gameConfig);
}