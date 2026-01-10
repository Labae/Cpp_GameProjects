#pragma once

#include "../../../GameLibrary/include/Components/Component.hpp"
#include "Services/EventService.hpp"

namespace GameLibrary
{
    class IInputProvider;
    class IGraphics;
    class AudioService;
    class FxService;
    struct EngineConfig;
    class SceneManager;
} // namespace GameLibrary

namespace SnakeGame
{
    struct SnakeGameConfig;
    struct GameData;
} // namespace SnakeGame

class GameManager final : public GameLibrary::Component
{
public:
    GameManager(
        GameLibrary::Actor* owner, GameLibrary::Actor& snakeActor,
        GameLibrary::EventService& eventSystem, GameLibrary::IInputProvider& input,
                GameLibrary::SceneManager& sceneManager, GameLibrary::AudioService& soundSystem,
                GameLibrary::EngineConfig& engineConfig, const SnakeGame::SnakeGameConfig& gameConfig,
                SnakeGame::GameData& gameData, GameLibrary::FxService& fxSystem);

    void Init() override;
    void Update(float deltaTime) override;
    void Render(GameLibrary::IGraphics& graphics) override;

    [[nodiscard]] int32_t GetScore() const noexcept { return m_score; }

private:
    void OnFoodEaten(const struct FoodEatenEvent& event);
    void OnGoldenFoodEaten(const struct GoldenFoodEatenEvent& event);
    void OnGameOver(const struct GameOverEvent& event);

    GameLibrary::EventService& m_eventSystem;
    GameLibrary::IInputProvider& m_input;
    GameLibrary::SceneManager& m_sceneManager;
    GameLibrary::AudioService& m_soundSystem;
    GameLibrary::EngineConfig& m_engineConfig;
    SnakeGame::GameData& m_gameData;
    GameLibrary::FxService& m_fxSystem;

    GameLibrary::SubscriptionToken m_goldenFoodEatenToken{};
    GameLibrary::SubscriptionToken m_foodEatenToken{};
    GameLibrary::SubscriptionToken m_gameOverToken{};

    GameLibrary::Actor& m_snakeActor;

    int32_t m_score{};
    float m_elapsedTime{};
    bool m_isGameOver{};
    float m_gameOverDelay{};
};