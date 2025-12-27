#include "FoodSpawner.hpp"
#include "Systems/EventSystem.hpp"
#include "Core/EngineConfig.hpp"
#include "Config/SnakeGameConfig.hpp"
#include "Events/GameEvents.hpp"
#include "GoldenPickup.hpp"
#include <random>
#include  <chrono>

FoodSpawner::FoodSpawner(GameLibrary::Transform& foodTransform,
    GoldenPickup& goldenPickup,
    GameLibrary::EventSystem& eventSystem,
    const GameLibrary::EngineConfig& engineConfig,
    const SnakeGame::SnakeGameConfig& gameConfig)
    : m_foodTransform(foodTransform)
    , m_goldenPickup(goldenPickup)
    , m_eventSystem(eventSystem)
    , m_engineConfig(engineConfig)
    , m_gameConfig(gameConfig)
{
}

void FoodSpawner::Init()
{
    m_foodEatenToken = m_eventSystem.Subscribe<FoodEatenEvent>(
        [this](const FoodEatenEvent& event) { OnFoodEaten(event); }
    );

    SpawnFood();
}

void FoodSpawner::SpawnFood()
{
    auto seed = static_cast<unsigned>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()
        );
    std::mt19937 gen(seed);

    int32_t maxX = (m_engineConfig.screenWidth / m_gameConfig.gridSize) - 1;
    int32_t maxY = (m_engineConfig.screenHeight / m_gameConfig.gridSize) - 1;

    std::uniform_int_distribution<> distX(0, maxX);
    std::uniform_int_distribution<> distY(0, maxY);

    m_foodTransform.position.x = static_cast<float>(distX(gen) * m_gameConfig.gridSize);
    m_foodTransform.position.y = static_cast<float>(distY(gen) * m_gameConfig.gridSize);
}

void FoodSpawner::OnFoodEaten(const FoodEatenEvent& event)
{
    SpawnFood();

    if (!m_goldenPickup.IsActive())
    {
        auto seed = static_cast<unsigned>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()
            );
        std::mt19937 gen(seed);

        std::uniform_int_distribution<int32_t> chanceDist(1, 5);
        if (chanceDist(gen) == 1)
        {
            SpawnGoldenFood();
        }
    }
}

void FoodSpawner::SpawnGoldenFood()
{
    auto seed = static_cast<unsigned>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()
        );
    std::mt19937 gen(seed);

    int32_t maxX = (m_engineConfig.screenWidth / m_gameConfig.gridSize) - 1;
    int32_t maxY = (m_engineConfig.screenHeight / m_gameConfig.gridSize) - 1;

    std::uniform_int_distribution<int32_t> distX(0, maxX);
    std::uniform_int_distribution<int32_t> distY(0, maxY);

    m_goldenPickup.Spawn(
        static_cast<float>(distX(gen) * m_gameConfig.gridSize),
        static_cast<float>(distY(gen) * m_gameConfig.gridSize));
}
