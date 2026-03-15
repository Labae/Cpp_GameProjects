//
// Created by Labae on 3/15/26.
//

#include "Scenes/DungeonScene.hpp"

#include "Core/ServiceContainer.hpp"
#include "Scenes/States/ExplorationState.hpp"

namespace TextRPG
{
    DungeonScene::DungeonScene(const std::string& name, GameLibrary::ServiceContainer& container)
        : Scene(name, container)
    {
    }

    void DungeonScene::OnEnter()
    {
        auto& container = GetContainer();
        m_input = container.Resolve<GameLibrary::IInputProvider>();
        m_sceneManager = container.Resolve<GameLibrary::SceneManager>();
        m_engineConfig = container.Resolve<GameLibrary::EngineConfig>();
        m_gameSession = container.Resolve<GameSessionData>();

        m_stateMachine.AddState<ExplorationState>();
        m_stateMachine.ChangeState<ExplorationState>();
    }

    void DungeonScene::OnExit()
    {

    }

    void DungeonScene::Update(float deltaTime)
    {
        m_stateMachine.Update(deltaTime);
    }

    void DungeonScene::Render(GameLibrary::IGraphics& graphics)
    {
        m_stateMachine.Render(graphics);
    }
} // namespace TextRPG