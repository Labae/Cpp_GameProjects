#pragma once

#include "Components/GoldenPickup.hpp"
#include "Components/Pickup.hpp"
#include "GameObject/Component.hpp"
#include "GameObject/Transform.hpp"
#include "Scene/Scene.hpp"
#include "Systems/EventSystem.hpp"

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
    FoodSpawner(GameLibrary::Scene& scene, GameLibrary::EventSystem& eventSystem,
                const GameLibrary::EngineConfig& engineConfig, const SnakeGame::SnakeGameConfig& gameConfig);

    void Init() override;
    void Update(float deltaTime) override;

private:
    void OnFoodEaten(const struct FoodEatenEvent& event);

    void SpawnFood();
    void SpawnGoldenFood();

    sf::Vector2f GetRandomPosition() const;

    GameLibrary::Scene& m_scene;
    GameLibrary::EventSystem& m_eventSystem;
    const GameLibrary::EngineConfig& m_engineConfig;
    const SnakeGame::SnakeGameConfig& m_gameConfig;

    GameLibrary::SubscriptionToken m_foodEatenToken{};

    struct GoldenFoodEntry
    {
        GameLibrary::GameObject* gameObject{};
        GoldenPickup* pickup{};
    };
    std::vector<GoldenFoodEntry> m_goldenFoods{};
};