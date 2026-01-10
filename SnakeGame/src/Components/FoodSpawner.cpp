#include "Components/FoodSpawner.hpp"

#include "Actor/BoxCollider.hpp"
#include "Components/GoldenPickup.hpp"
#include "Components/Pickup.hpp"
#include "Config/SnakeGameConfig.hpp"
#include "Core/EngineConfig.hpp"
#include "Core/ServiceContainer.hpp"
#include "Events/GameEvents.hpp"
#include "Scene/Scene.hpp"
#include "Services/PhysicsService.hpp"

#include <chrono>
#include <random>

FoodSpawner::FoodSpawner(GameLibrary::Actor* owner, GameLibrary::Scene& scene, GameLibrary::EventService& eventSystem,
                         const GameLibrary::EngineConfig& engineConfig, const SnakeGame::SnakeGameConfig& gameConfig)
    : Component(owner)
    , m_scene(scene)
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
            it->actor->Destroy();
            it = m_goldenFoods.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void FoodSpawner::SpawnFood() const
{
    auto* food = m_scene.CreateActor();
    food->GetTransform().position = GetRandomPosition();

    auto* pickup =
        food->AddComponent<Pickup>(m_eventSystem, m_gameConfig.gridSize);

    auto* physicsService = m_scene.GetContainer().Resolve<GameLibrary::PhysicsService>();
    food->AddComponent<GameLibrary::BoxCollider>(*physicsService, m_gameConfig.gridSize, m_gameConfig.gridSize,
        [pickup, food](GameLibrary::Actor* other)
        {
            pickup->OnCollision(other);
            food->Destroy();
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
    auto* foodGameObject = m_scene.CreateActor();
    foodGameObject->GetTransform().position = GetRandomPosition();

    auto* goldenFood = foodGameObject->AddComponent<GoldenPickup>(m_eventSystem,
                                                                  m_gameConfig.gridSize);

    auto* physicService = m_scene.GetContainer().Resolve<GameLibrary::PhysicsService>();
    foodGameObject->AddComponent<GameLibrary::BoxCollider>(*physicService, m_gameConfig.gridSize, m_gameConfig.gridSize,
        [goldenFood](GameLibrary::Actor* other) { goldenFood->OnCollision(other); });

    m_goldenFoods.push_back({foodGameObject, goldenFood});
}

sf::Vector2f FoodSpawner::GetRandomPosition() const
{
    static std::mt19937 gen{std::random_device{}()};

    const int32_t maxX = (m_engineConfig.screenWidth / m_gameConfig.gridSize) - 1;
    const int32_t maxY = (m_engineConfig.screenHeight / m_gameConfig.gridSize) - 1;

    std::uniform_int_distribution<int32_t> distX(0, maxX);
    std::uniform_int_distribution<int32_t> distY(0, maxY);

    return {static_cast<float>(distX(gen) * m_gameConfig.gridSize), static_cast<float>(distY(gen) * m_gameConfig.gridSize)};
}