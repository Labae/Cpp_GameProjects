#pragma once

#include "GameObject/Component.hpp"
#include "Systems/EventSystem.hpp"

namespace GameLibrary
{
    class IInputProvider;
    class ISceneManager;
    class IGraphics;
    class SoundSystem;
    class FxSystem;
    struct EngineConfig;
}

namespace SnakeGame
{
    struct SnakeGameConfig;
    struct GameData;
}

class GameManager : public GameLibrary::Component
{
public:
    GameManager(GameLibrary::EventSystem& eventSystem,
        GameLibrary::IInputProvider& input,
        GameLibrary::ISceneManager& sceneManager,
        GameLibrary::SoundSystem& soundSystem,
        GameLibrary::EngineConfig& engineConfig,
        SnakeGame::SnakeGameConfig& gameConfig,
        SnakeGame::GameData& gameData,
        GameLibrary::FxSystem& fxSystem);

    void Init() override;
    void Update(float deltaTime) override;
    void Render(GameLibrary::IGraphics& graphics) override;

    [[nodiscard]] int32_t GetScore() const noexcept { return m_score; }

private:
    void OnScoreChanged(const struct ScoreChangedEvent& event);
    void OnFoodEaten(const struct FoodEatenEvent& event);
    void OnGoldenFoodEaten(const struct GoldenFoodEatenEvent& event);
    void OnGameOver(const struct GameOverEvent& event);

    GameLibrary::EventSystem& m_eventSystem;
    GameLibrary::IInputProvider& m_input;
    GameLibrary::ISceneManager& m_sceneManager;
    GameLibrary::SoundSystem& m_soundSystem;
    GameLibrary::EngineConfig& m_engineConfig;
    SnakeGame::GameData& m_gameData;
    GameLibrary::FxSystem& m_fxSystem;

    GameLibrary::SubscriptionToken m_scoreChangedToken;
    GameLibrary::SubscriptionToken m_goldenFoodEatenToken;
    GameLibrary::SubscriptionToken m_foodEatenToken;
    GameLibrary::SubscriptionToken m_gameOverToken;

    int32_t m_score{};
    float m_elapsedTime{};
    bool m_isGameOver{};
    float m_gameOverDelay{};
};