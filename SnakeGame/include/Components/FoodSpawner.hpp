#pragma once

#include "GameObject/Component.hpp"
#include "GameObject/Transform.hpp"
#include "Systems/EventSystem.hpp"

class GoldenPickup;

namespace GameLibrary
{
    struct EngineConfig;
}

namespace SnakeGame
{
    struct SnakeGameConfig;
}

class FoodSpawner : public GameLibrary::Component
{
public:
    FoodSpawner(GameLibrary::Transform& foodTransform, GoldenPickup& goldenPickup,
                GameLibrary::EventSystem& eventSystem, const GameLibrary::EngineConfig& engineConfig,
                const SnakeGame::SnakeGameConfig& gameConfig);

    void Init() override;
    void SpawnFood();

private:
    void OnFoodEaten(const struct FoodEatenEvent& event);
    void SpawnGoldenFood();

    GameLibrary::Transform& m_foodTransform;
    GoldenPickup& m_goldenPickup;

    GameLibrary::EventSystem& m_eventSystem;
    const GameLibrary::EngineConfig& m_engineConfig;
    const SnakeGame::SnakeGameConfig& m_gameConfig;

    GameLibrary::SubscriptionToken m_foodEatenToken{};
};