#include "GameManager.hpp"
#include "Systems/EventSystem.hpp"
#include "Systems/SoundSystem.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Interfaces/ISceneManager.hpp"
#include "Interfaces/IGraphics.hpp"
#include "Core/EngineConfig.hpp"
#include "Events/GameEvents.hpp"
#include "Data/GameData.hpp"
#include "Config/SnakeGameConfig.hpp"
#include "Systems/FxSystem.hpp"
#include "Fx/ParticleFx.hpp"
#include "Fx/ShakeFx.hpp"
#include "Fx/FlashFx.hpp"
#include <format>

GameManager::GameManager(GameLibrary::EventSystem& eventSystem,
    GameLibrary::IInputProvider& input,
    GameLibrary::ISceneManager& sceneManager,
    GameLibrary::SoundSystem& soundSystem,
    GameLibrary::EngineConfig& engineConfig,
    SnakeGame::SnakeGameConfig& gameConfig,
    SnakeGame::GameData& gameData,
    GameLibrary::FxSystem& fxSystem)
    : m_eventSystem(eventSystem)
    , m_input(input)
    , m_sceneManager(sceneManager)
    , m_soundSystem(soundSystem)
    , m_engineConfig(engineConfig)
    , m_gameData(gameData)
    , m_fxSystem(fxSystem)
{
    m_gameOverDelay = gameConfig.gameOverDelay;
}

void GameManager::Init()
{
    m_scoreChangedToken = m_eventSystem.Subscribe<ScoreChangedEvent>(
        [this](const ScoreChangedEvent& event) { OnScoreChanged(event); }
    );

    m_foodEatenToken = m_eventSystem.Subscribe<FoodEatenEvent>(
        [this](const FoodEatenEvent& event) { OnFoodEaten(event); }
    );

    m_goldenFoodEatenToken = m_eventSystem.Subscribe<GoldenFoodEatenEvent>(
        [this](const GoldenFoodEatenEvent& event) { OnGoldenFoodEaten(event); }
    );

    m_gameOverToken = m_eventSystem.Subscribe<GameOverEvent>(
        [this](const GameOverEvent& event) { OnGameOver(event); }
    );
}

void GameManager::Update(float deltaTime)
{
    // 게임 중 추가 로직이 필요하면 여기에
    if (!m_isGameOver)
    {
        m_elapsedTime += deltaTime;
    }
    else
    {
        m_gameOverDelay -= deltaTime;
        if (m_gameOverDelay <= 0.0f)
        {
            m_sceneManager.LoadScene("Result");
        }
    }
}

void GameManager::Render(GameLibrary::IGraphics& graphics)
{
    // 점수 표시 (좌상단)
    std::wstring scoreText = std::format(L"Score: {}", m_score);
    graphics.DrawLabel(scoreText.c_str(), 10, 10, 200, 24, { 255, 255, 255, 255 });

    // 타이머 표시 (중앙 상단)
    auto minutes = static_cast<int32_t>(m_elapsedTime) / 60;
    auto seconds = static_cast<int32_t>(m_elapsedTime) % 60;
    std::wstring timeText = std::format(L"{:02}:{:02}", minutes, seconds);
    graphics.DrawLabel(timeText.c_str(), (m_engineConfig.screenWidth - 100) / 2, 10, 90, 24, {255, 255, 255, 255}, GameLibrary::TextAlign::Center);

    // 최고 점수 표시 (우상단)
    std::wstring highScoreText = std::format(L"Best: {}", m_gameData.highScore);
    graphics.DrawLabel(highScoreText.c_str(), m_engineConfig.screenWidth - 200, 10, 190, 24, { 180, 180, 180, 255 }, GameLibrary::TextAlign::Right);
}

void GameManager::OnScoreChanged(const ScoreChangedEvent& event)
{
    m_score = event.score;
}

void GameManager::OnFoodEaten(const FoodEatenEvent& event)
{
    m_soundSystem.Play("eat");

    // 파티클 이펙트 생성 (음식 위치)
    GameLibrary::ParticleFxConfig config
    {
        .color = { 255, 0, 0, 255 },
        .count = 12,
        .duration = 1.0f,
        .size = 10.0f,
        .speed = 200.0f,
        .startAlpha = 255,
        .endAlpha = 100
    };
    m_fxSystem.Spawn<GameLibrary::ParticleFx>(event.x, event.y, config);

    m_fxSystem.Spawn<GameLibrary::ShakeFx>(3.0f, 0.5f);
}

void GameManager::OnGoldenFoodEaten(const GoldenFoodEatenEvent& event)
{
    m_score += 5;
    m_soundSystem.Play("coin");

    // 파티클 이펙트 생성 (음식 위치)
    GameLibrary::ParticleFxConfig config
    {
        .color = { 255, 200, 0, 255 },
        .count = 16,
        .duration = 1.0f,
        .size = 10.0f,
        .speed = 200.0f,
        .startAlpha = 255,
        .endAlpha = 150
    };
    m_fxSystem.Spawn<GameLibrary::ParticleFx>(event.x, event.y, config);

    m_fxSystem.Spawn<GameLibrary::ShakeFx>(3.0f, 0.5f);
}

void GameManager::OnGameOver(const GameOverEvent& event)
{
    m_isGameOver = true;
    m_gameData.lastScore = event.score;
    m_soundSystem.Play("gameover");

    m_fxSystem.Spawn<GameLibrary::ShakeFx>(10.0f, 1.0f);

    // 빨간 플래시 이펙트
    m_fxSystem.Spawn<GameLibrary::FlashFx>(m_engineConfig.screenWidth, m_engineConfig.screenHeight, GameLibrary::Color{ 255, 0, 0, 180 }, 0.3f, 5);
}