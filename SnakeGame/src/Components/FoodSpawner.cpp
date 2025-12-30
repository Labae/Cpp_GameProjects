#include "Components/FoodSpawner.hpp"

#include "Components/GoldenPickup.hpp"
#include "Config/SnakeGameConfig.hpp"
#include "Core/EngineConfig.hpp"
#include "Core/ServiceContainer.hpp"
#include "Events/GameEvents.hpp"
#include "GameObject/BoxCollider.hpp"
#include "Interfaces/IPhysicsSystem.hpp"
#include "Scene/Scene.hpp"

#include <chrono>
#include <random>

FoodSpawner::FoodSpawner(GameLibrary::Scene& scene, GameLibrary::EventSystem& eventSystem,
                         const GameLibrary::EngineConfig& engineConfig, const SnakeGame::SnakeGameConfig& gameConfig)
    : m_scene(scene)
    , m_eventSystem(eventSystem)
    , m_engineConfig(engineConfig)
    , m_gameConfig(gameConfig)
{
}

void FoodSpawner::Init()
{
    m_foodEatenToken =
        m_eventSystem.Subscribe<FoodEatenEvent>([this](const FoodEatenEvent& event) { OnFoodEaten(event); });

    SpawnFood();
}

void FoodSpawner::Update([[maybe_unused]] float deltaTime)
{
    for (auto it = m_goldenFoods.begin(); it != m_goldenFoods.end();)
    {
        if (it->pickup->IsExpired() || it->pickup->IsEaten())
        {
            m_scene.DestroyGameObject(it->gameObject);
            it = m_goldenFoods.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void FoodSpawner::SpawnFood()
{
    auto* foodGameObject = m_scene.CreateGameObject();
    foodGameObject->GetTransform().position = GetRandomPosition();

    auto* pickup =
        foodGameObject->AddComponent<Pickup>(foodGameObject->GetTransform(), m_eventSystem, m_gameConfig.gridSize);

    auto* physicSystem = m_scene.GetContainer().Resolve<GameLibrary::IPhysicsSystem>();
    foodGameObject->AddComponent<GameLibrary::BoxCollider>(
        foodGameObject->GetTransform(), *physicSystem, m_gameConfig.gridSize, m_gameConfig.gridSize,
        [pickup, foodGameObject, this](GameLibrary::ICollidable* other)
        {
            pickup->OnCollision(other);
            m_scene.DestroyGameObject(foodGameObject);
        });
}

void FoodSpawner::OnFoodEaten([[maybe_unused]] const FoodEatenEvent& event)
{
    SpawnFood();

    static std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<int32_t> chanceDist(1, 5);
    if (chanceDist(gen) == 1)
    {
        SpawnGoldenFood();
    }
}

void FoodSpawner::SpawnGoldenFood()
{
    auto* foodGameObject = m_scene.CreateGameObject();
    foodGameObject->GetTransform().position = GetRandomPosition();

    auto* goldenFood = foodGameObject->AddComponent<GoldenPickup>(foodGameObject->GetTransform(), m_eventSystem,
                                                                  m_gameConfig.gridSize);

    auto* physicSystem = m_scene.GetContainer().Resolve<GameLibrary::IPhysicsSystem>();
    foodGameObject->AddComponent<GameLibrary::BoxCollider>(
        foodGameObject->GetTransform(), *physicSystem, m_gameConfig.gridSize, m_gameConfig.gridSize,
        [goldenFood](GameLibrary::ICollidable* other) { goldenFood->OnCollision(other); });

    m_goldenFoods.push_back({foodGameObject, goldenFood});
}

sf::Vector2f FoodSpawner::GetRandomPosition() const
{
    static std::mt19937 gen{std::random_device{}()};

    const int32_t maxX = (m_engineConfig.screenWidth / m_gameConfig.gridSize) - 1;
    const int32_t maxY = (m_engineConfig.screenHeight / m_gameConfig.gridSize) - 1;

    std::uniform_int_distribution<int32_t> distX(0, maxX);
    std::uniform_int_distribution<int32_t> distY(0, maxY);

    return sf::Vector2f((distX(gen) * m_gameConfig.gridSize), (distY(gen) * m_gameConfig.gridSize));
}