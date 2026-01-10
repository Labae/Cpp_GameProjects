#include "Scenes/GameScene.hpp"

#include "../../../GameLibrary/include/Components/BoxCollider.hpp"
#include "Components/FoodSpawner.hpp"
#include "Components/GameManager.hpp"
#include "Components/GridBackground.hpp"
#include "Components/SnakeController.hpp"
#include "Config/SnakeGameConfig.hpp"
#include "Controllers/SnakePlayerController.hpp"
#include "Core/EngineConfig.hpp"
#include "Core/ServiceContainer.hpp"
#include "Data/GameData.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Scene/SceneManager.hpp"
#include "Services/AudioService.hpp"
#include "Services/ConfigService.hpp"
#include "Services/EventService.hpp"
#include "Services/FxService.hpp"
#include "Services/PhysicsService.hpp"
#include "Services/SaveService.hpp"
#include "Systems/Logger.hpp"

namespace
{
    constexpr auto GAME_CONFIG_PATH = "game.yaml";
}

GameScene::GameScene(const std::string& name, GameLibrary::ServiceContainer& container) : Scene(name, container) {}
GameScene::~GameScene() = default;

void GameScene::OnEnter()
{
    m_pauseState = PauseState::None;
    m_menuIndex = 0;
    m_settingsIndex = 0;

    auto& container = GetContainer();
    m_fxSystem = container.Resolve<GameLibrary::FxService>();
    m_input = container.Resolve<GameLibrary::IInputProvider>();
    m_engineConfig = container.Resolve<GameLibrary::EngineConfig>();
    m_audioService = container.Resolve<GameLibrary::AudioService>();
    m_saveSystem = container.Resolve<GameLibrary::SaveService>();
    m_gameData = container.Resolve<SnakeGame::GameData>();

    if (m_audioService && m_gameData)
    {
        m_audioService->SetBGMVolume(m_gameData->bgmVolume);
        m_audioService->SetSFXVolume(m_gameData->sfxVolume);
    }

    ApplyConfig();
    SetupGame();

    if (m_audioService)
    {
        if (not m_audioService->PlayBGM("assets/sounds/background.wav"))
        {
            GameLibrary::Logger::Warning("Failed to load BGM: background");
        }

        GameLibrary::Logger::Info("BGM loaded + " + std::to_string(m_audioService->GetBGMVolume()));
    }
}

void GameScene::OnExit()
{
    if (m_audioService)
    {
        m_audioService->StopBGM();
    }

    if (m_playerController)
    {
        m_playerController->UnPossess();
        m_playerController.reset();
    }

    Clear();
    m_fxSystem->Clear();
}

void GameScene::Update(float deltaTime)
{
    if (!m_input)
    {
        return;
    }

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
            if (m_playerController)
            {
                m_playerController->Update(deltaTime);
            }

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
        m_menuIndex = (m_menuIndex + 2) % 3;
    }
    if (m_input->IsKeyPressed(GameLibrary::KeyCode::Down))
    {
        m_menuIndex = (m_menuIndex + 1) % 3;
    }
    if (m_input->IsKeyPressed(GameLibrary::KeyCode::Enter) || m_input->IsKeyPressed(GameLibrary::KeyCode::Space))
    {
        switch (m_menuIndex)
        {
        case 0: // Resume
        {
            m_pauseState = PauseState::None;
            break;
        }
        case 1: // Settings
        {
            m_pauseState = PauseState::Settings;
            m_settingsIndex = 0;
            break;
        }
        case 2: // Quit
        {
            if (auto* sceneManager = GetContainer().Resolve<GameLibrary::SceneManager>())
            {
                sceneManager->LoadScene("Title");
            }
            break;
        }
        default:;
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

    constexpr float delta = 0.1f;
    bool changed = false;

    if (m_input->IsKeyPressed(GameLibrary::KeyCode::Left))
    {
        if (m_audioService)
        {
            if (m_settingsIndex == 0)
            {
                m_audioService->SetBGMVolume(m_audioService->GetBGMVolume() - delta);
            }
            else
            {
                m_audioService->SetSFXVolume(m_audioService->GetSFXVolume() - delta);
            }
            changed = true;
        }
    }
    if (m_input->IsKeyPressed(GameLibrary::KeyCode::Right))
    {
        if (m_audioService)
        {
            if (m_settingsIndex == 0)
            {
                m_audioService->SetBGMVolume(m_audioService->GetBGMVolume() + delta);
            }
            else
            {
                m_audioService->SetSFXVolume(m_audioService->GetSFXVolume() + delta);
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

void GameScene::SaveSettings() const
{
    if (not m_gameData or not m_saveSystem)
    {
        return;
    }

    m_gameData->bgmVolume = m_audioService->GetBGMVolume();
    m_gameData->sfxVolume = m_audioService->GetSFXVolume();

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

void GameScene::RenderPauseMenu(GameLibrary::IGraphics& graphics) const
{
    if (!m_engineConfig)
    {
        return;
    }

    const int32_t screenW = m_engineConfig->screenWidth;
    const int32_t screenH = m_engineConfig->screenHeight;

    // 반투명 오버레이
    graphics.FillRect(0, 0, screenW, screenH, sf::Color(0, 0, 0, 150));

    // 일시정지 박스
    constexpr int32_t boxW = 300;
    constexpr int32_t boxH = 200;
    const int32_t boxX = (screenW - boxW) / 2;
    const int32_t boxY = (screenH - boxH) / 2;

    graphics.FillRect(boxX, boxY, boxW, boxH, sf::Color(40, 40, 40, 255));
    graphics.DrawRect(boxX, boxY, boxW, boxH, sf::Color(255, 255, 255, 255));

    // 타이틀
    graphics.DrawLabel("PAUSED", boxX + boxW / 2, boxY + 20, 36, sf::Color(255, 255, 255, 255),
                       GameLibrary::TextAlign::Center);

    // 메뉴 항목
    for (int32_t i = 0; i < 3; ++i)
    {
        const char* items[] = {"Resume", "Settings", "Quit"};
        const sf::Color color = (i == m_menuIndex) ? sf::Color(255, 255, 0, 255) : sf::Color(180, 180, 180, 255);
        graphics.DrawLabel(items[i], boxX + boxW / 2, boxY + 70 + i * 35, 24, color, GameLibrary::TextAlign::Center);
    }
}

void GameScene::RenderSettings(GameLibrary::IGraphics& graphics) const
{
    if (!m_engineConfig)
    {
        return;
    }

    const int32_t screenW = m_engineConfig->screenWidth;
    const int32_t screenH = m_engineConfig->screenHeight;

    // 반투명 오버레이
    graphics.FillRect(0, 0, screenW, screenH, sf::Color(0, 0, 0, 150));

    // 설정 박스
    constexpr int32_t boxW = 350;
    constexpr int32_t boxH = 200;
    const int32_t boxX = (screenW - boxW) / 2;
    const int32_t boxY = (screenH - boxH) / 2;

    graphics.FillRect(boxX, boxY, boxW, boxH, sf::Color(40, 40, 40, 255));
    graphics.DrawRect(boxX, boxY, boxW, boxH, sf::Color(255, 255, 255, 255));

    // 타이틀
    graphics.DrawLabel("SETTINGS", boxX + boxW / 2, boxY + 20, 36, sf::Color(255, 255, 255, 255),
                       GameLibrary::TextAlign::Center);

    // 볼륨 바
    const float bgmVol = m_audioService ? m_audioService->GetBGMVolume() : 1.0f;
    const float sfxVol = m_audioService ? m_audioService->GetSFXVolume() : 1.0f;

    const int32_t barX = boxX + 150;
    constexpr int32_t barW = 150;
    constexpr int32_t barH = 20;

    // BGM
    const sf::Color bgmColor = (m_settingsIndex == 0) ? sf::Color(255, 255, 0, 255) : sf::Color(180, 180, 180, 255);
    graphics.DrawLabel("BGM", boxX + 30, boxY + 70, 24, bgmColor, GameLibrary::TextAlign::Left);
    graphics.DrawRect(barX, boxY + 70, barW, barH, sf::Color(100, 100, 100, 255));
    graphics.FillRect(barX, boxY + 70, static_cast<int32_t>(barW * bgmVol), barH, sf::Color(0, 200, 0, 255));

    // SFX
    const sf::Color sfxColor = (m_settingsIndex == 1) ? sf::Color(255, 255, 0, 255) : sf::Color(180, 180, 180, 255);
    graphics.DrawLabel("SFX", boxX + 30, boxY + 110, 24, sfxColor, GameLibrary::TextAlign::Left);
    graphics.DrawRect(barX, boxY + 110, barW, barH, sf::Color(100, 100, 100, 255));
    graphics.FillRect(barX, boxY + 110, static_cast<int32_t>(barW * sfxVol), barH, sf::Color(0, 200, 0, 255));

    // 안내
    graphics.DrawLabel("< > to adjust, ESC to back", boxX + boxW / 2, boxY + 160, 18, sf::Color(120, 120, 120, 255),
                       GameLibrary::TextAlign::Center);
}

void GameScene::ApplyConfig() const
{
    auto& container = GetContainer();
    auto* configSystem = container.Resolve<GameLibrary::ConfigService>();
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
    auto* sceneManager = container.Resolve<GameLibrary::SceneManager>();
    auto* eventSystem = container.Resolve<GameLibrary::EventService>();
    auto* soundSystem = container.Resolve<GameLibrary::AudioService>();
    auto* physicsSystem = container.Resolve<GameLibrary::PhysicsService>();

    // GridBackground
    auto* grid = CreateActor();
    grid->AddComponent<GridBackground>(engineConfig->screenWidth, engineConfig->screenHeight, gameConfig->gridSize);

    // Snake
    auto* snake = CreateActor<GameLibrary::Pawn>();
    snake->SetTag("Snake");

    snake->GetTransform().position = {
        static_cast<float>((engineConfig->screenWidth / 2 / gameConfig->gridSize) * gameConfig->gridSize),
        static_cast<float>((engineConfig->screenHeight / 2 / gameConfig->gridSize) * gameConfig->gridSize)};

    snake->AddComponent<GameLibrary::GridMovementComponent>(gameConfig->gridSize, gameConfig->moveInterval);
    auto* snakeController = snake->AddComponent<SnakeController>(*eventSystem, *engineConfig, *gameConfig);

    snake->AddComponent<GameLibrary::BoxCollider>(*physicsSystem, gameConfig->gridSize, gameConfig->gridSize,
                                                  [snakeController](GameLibrary::Actor* other)
                                                  { snakeController->OnCollision(other); });

    m_playerController = std::make_unique<SnakeGame::SnakePlayerController>(*input);
    m_playerController->Possess(snake);

    // FoodSpawner
    auto* spawner = CreateActor();
    spawner->AddComponent<FoodSpawner>(*this, *eventSystem, *engineConfig, *gameConfig);

    // GameManager
    auto* manager = CreateActor();
    manager->AddComponent<GameManager>(*snake, *eventSystem, *input, *sceneManager, *soundSystem, *engineConfig, *gameConfig,
                                       *gameData, *m_fxSystem);
}