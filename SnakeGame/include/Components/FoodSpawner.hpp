#pragma once

#include "Actor/Component.hpp"
#include "Actor/Transform.hpp"
#include "Services/EventService.hpp"
#include "Components/GoldenPickup.hpp"
#include "Scene/Scene.hpp"

namespace GameLibrary
{
    struct EngineConfig;
}

namespace SnakeGame
{
    struct SnakeGameConfig;
}

class FoodSpawner final : public GameLibrary::Component
{
public:
    FoodSpawner(GameLibrary::Actor* owner, GameLibrary::Scene& scene, GameLibrary::EventService& eventSystem,
                const GameLibrary::EngineConfig& engineConfig, const SnakeGame::SnakeGameConfig& gameConfig);

    void Init() override;
    void Update(float deltaTime) override;

private:
    void OnFoodEaten(const struct FoodEatenEvent& event);

    void SpawnFood() const;
    void SpawnGoldenFood();

    [[nodiscard]] sf::Vector2f GetRandomPosition() const;

    GameLibrary::Scene& m_scene;
    GameLibrary::EventService& m_eventSystem;
    const GameLibrary::EngineConfig& m_engineConfig;
    const SnakeGame::SnakeGameConfig& m_gameConfig;

    GameLibrary::SubscriptionToken m_foodEatenToken{};

    struct GoldenFoodEntry
    {
        GameLibrary::Actor* actor{};
        GoldenPickup* pickup{};
    };
    std::vector<GoldenFoodEntry> m_goldenFoods{};
};