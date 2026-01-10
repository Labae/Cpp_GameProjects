#include "Components/GameManager.hpp"

#include "Config/SnakeGameConfig.hpp"
#include "Core/EngineConfig.hpp"
#include "Data/GameData.hpp"
#include "Events/GameEvents.hpp"
#include "Fx/FlashFx.hpp"
#include "Fx/ParticleFx.hpp"
#include "Fx/ShakeFx.hpp"
#include "Interfaces/IGraphics.hpp"
#include "Scene/SceneManager.hpp"
#include "Services/AudioService.hpp"
#include "Services/FxService.hpp"

#include <format>

GameManager::GameManager(GameLibrary::Actor* owner, GameLibrary::Actor& snakeActor,
                         GameLibrary::EventService& eventSystem, GameLibrary::IInputProvider& input,
                         GameLibrary::SceneManager& sceneManager, GameLibrary::AudioService& soundSystem,
                         GameLibrary::EngineConfig& engineConfig, const SnakeGame::SnakeGameConfig& gameConfig,
                         SnakeGame::GameData& gameData, GameLibrary::FxService& fxSystem)
    : Component(owner)
    , m_eventSystem(eventSystem)
    , m_input(input)
    , m_sceneManager(sceneManager)
    , m_soundSystem(soundSystem)
    , m_engineConfig(engineConfig)
    , m_gameData(gameData)
    , m_fxSystem(fxSystem)
    , m_snakeActor(snakeActor)
    , m_gameOverDelay(gameConfig.gameOverDelay)
{
}

void GameManager::Init()
{
    m_foodEatenToken =
        m_eventSystem.Subscribe<FoodEatenEvent>([this](const FoodEatenEvent& event) { OnFoodEaten(event); });

    m_goldenFoodEatenToken = m_eventSystem.Subscribe<GoldenFoodEatenEvent>([this](const GoldenFoodEatenEvent& event)
                                                                           { OnGoldenFoodEaten(event); });

    m_gameOverToken = m_eventSystem.Subscribe<GameOverEvent>([this](const GameOverEvent& event) { OnGameOver(event); });
}

void GameManager::Update(const float deltaTime)
{
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
    const std::string scoreText = std::format("Score: {}", m_score);
    graphics.DrawLabel(scoreText, 10, 10, 24, sf::Color(255, 255, 255, 255));

    // 타이머 표시 (중앙 상단)
    const auto minutes = static_cast<int32_t>(m_elapsedTime) / 60;
    const auto seconds = static_cast<int32_t>(m_elapsedTime) % 60;
    const std::string timeText = std::format("{:02}:{:02}", minutes, seconds);
    graphics.DrawLabel(timeText, m_engineConfig.screenWidth / 2, 10, 24, sf::Color(255, 255, 255, 255),
                       GameLibrary::TextAlign::Center);

    // 최고 점수 표시 (우상단)
    const std::string highScoreText = std::format("Best: {}", m_gameData.highScore);
    graphics.DrawLabel(highScoreText, m_engineConfig.screenWidth - 10, 10, 24, sf::Color(180, 180, 180, 255),
                       GameLibrary::TextAlign::Right);
}

void GameManager::OnFoodEaten(const FoodEatenEvent& event)
{
    m_score++;
    m_soundSystem.Play("eat");

    GameLibrary::ParticleFxConfig config{
        .color = sf::Color(255, 0, 0, 255),
        .count = 12,
        .duration = 1.0f,
        .size = 10.0f,
        .speed = 200.0f,
        .startAlpha = 255,
        .endAlpha = 100,
    };
    m_fxSystem.Spawn<GameLibrary::ParticleFx>(event.x, event.y, config);
    m_fxSystem.Spawn<GameLibrary::ShakeFx>(3.0f, 0.5f);
}

void GameManager::OnGoldenFoodEaten(const GoldenFoodEatenEvent& event)
{
    m_score += 5;
    m_soundSystem.Play("coin");

    GameLibrary::ParticleFxConfig config{
        .color = sf::Color(255, 200, 0, 255),
        .count = 16,
        .duration = 1.0f,
        .size = 10.0f,
        .speed = 200.0f,
        .startAlpha = 255,
        .endAlpha = 150,
    };
    m_fxSystem.Spawn<GameLibrary::ParticleFx>(event.x, event.y, config);
    m_fxSystem.Spawn<GameLibrary::ShakeFx>(3.0f, 0.5f);
}

void GameManager::OnGameOver([[maybe_unused]] const GameOverEvent& event)
{
    m_isGameOver = true;
    m_gameData.lastScore = m_score;
    m_soundSystem.Play("gameover");

    m_snakeActor.SetUpdateEnabled(false);

    m_fxSystem.Spawn<GameLibrary::ShakeFx>(10.0f, 1.0f);
    m_fxSystem.Spawn<GameLibrary::FlashFx>(m_engineConfig.screenWidth, m_engineConfig.screenHeight,
                                           sf::Color(255, 0, 0, 180), 0.3f, 5);
}