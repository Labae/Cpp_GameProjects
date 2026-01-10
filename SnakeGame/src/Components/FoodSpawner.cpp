#include "Components/FoodSpawner.hpp"

#include "Actor/AutoGridMovementComponent.hpp"
#include "Actor/BoxCollider.hpp"
#include "Actor/LinearMovementComponent.hpp"
#include "Actor/MovementComponent.hpp"
#include "Components/GoldenPickup.hpp"
#include "Components/MovingPickup.hpp"
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

    for (auto it = m_movingFoods.begin(); it != m_movingFoods.end();)
    {
        if (it->pickup->IsEaten())
        {
            it->actor->Destroy();
            it = m_movingFoods.erase(it);
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

    auto* pickup = food->AddComponent<Pickup>(m_eventSystem, m_gameConfig.gridSize);

    auto* physicsService = m_scene.GetContainer().Resolve<GameLibrary::PhysicsService>();
    food->AddComponent<GameLibrary::BoxCollider>(*physicsService, m_gameConfig.gridSize, m_gameConfig.gridSize,
                                                 [pickup](const GameLibrary::Actor* other)
                                                 { pickup->OnCollision(other); });
}

void FoodSpawner::OnFoodEaten([[maybe_unused]] const FoodEatenEvent& event)
{
    SpawnFood();

    static std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<int32_t> chanceDist(1, 10);
    SpawnMovingFood();
    // if (const int32_t chance = chanceDist(gen); chance <= 2)
    // {
    //     SpawnGoldenFood();
    // }
    // else if (chance == 3)
    // {
    //     SpawnMovingFood();
    // }
}

void FoodSpawner::SpawnGoldenFood()
{
    auto* foodGameObject = m_scene.CreateActor();
    foodGameObject->GetTransform().position = GetRandomPosition();

    auto* goldenFood = foodGameObject->AddComponent<GoldenPickup>(m_eventSystem, m_gameConfig.gridSize);

    auto* physicService = m_scene.GetContainer().Resolve<GameLibrary::PhysicsService>();
    foodGameObject->AddComponent<GameLibrary::BoxCollider>(*physicService, m_gameConfig.gridSize, m_gameConfig.gridSize,
                                                           [goldenFood](const GameLibrary::Actor* other)
                                                           { goldenFood->OnCollision(other); });

    m_goldenFoods.push_back({foodGameObject, goldenFood});
}
void FoodSpawner::SpawnMovingFood()
{
    auto* food = m_scene.CreateActor();
    food->GetTransform().position = GetRandomPosition();

    static std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<int32_t> dirDist(0, 3);

    constexpr sf::Vector2f directions[] = {
        {1.0f, -1.0f},
        {1.0f, -1.0f},
        {-1.0f, -1.0f},
        {-1.0f, -1.0f},
    };

    auto* movement = food->AddComponent<GameLibrary::AutoGridMovementComponent>(m_gameConfig.gridSize,
                                                                                m_gameConfig.moveInterval * 2.0f);
    movement->SetDirection(directions[dirDist(gen)]);

    auto* pickup = food->AddComponent<MovingPickup>(m_eventSystem, m_engineConfig, m_gameConfig.gridSize);

    const auto physicsService = m_scene.GetContainer().Resolve<GameLibrary::PhysicsService>();
    food->AddComponent<GameLibrary::BoxCollider>(*physicsService, m_gameConfig.gridSize, m_gameConfig.gridSize,
                                                 [pickup](const GameLibrary::Actor* other)
                                                 { pickup->OnCollision(other); });

    m_movingFoods.push_back({food, pickup});
}

sf::Vector2f FoodSpawner::GetRandomPosition() const
{
    static std::mt19937 gen{std::random_device{}()};

    const int32_t maxX = (m_engineConfig.screenWidth / m_gameConfig.gridSize) - 2;
    const int32_t maxY = (m_engineConfig.screenHeight / m_gameConfig.gridSize) - 2;

    std::uniform_int_distribution<int32_t> distX(1, maxX);
    std::uniform_int_distribution<int32_t> distY(1, maxY);

    return {static_cast<float>(distX(gen) * m_gameConfig.gridSize),
            static_cast<float>(distY(gen) * m_gameConfig.gridSize)};
}