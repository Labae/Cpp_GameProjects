//
// Created by Labae on 3/14/26.
//

#include "Scenes/States/PauseMenuState.hpp"

#include "Scenes/GameScene.hpp"
#include "Scenes/States/PlayingState.hpp"
#include "Scenes/States/SettingsState.hpp"
#include "Core/ServiceContainer.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Scene/SceneManager.hpp"

void PauseMenuState::OnEnter([[maybe_unused]] GameScene& scene)
{
    m_menuIndex = 0;
}

void PauseMenuState::Update(GameScene& scene, [[maybe_unused]] const float deltaTime)
{
    auto& input = scene.GetInput();

    if (input.IsKeyPressed(GameLibrary::KeyCode::Escape))
    {
        scene.GetStateMachine().ChangeState<PlayingState>();
        return;
    }

    if (input.IsKeyPressed(GameLibrary::KeyCode::Up))
    {
        m_menuIndex = (m_menuIndex + 2) % 3;
    }
    if (input.IsKeyPressed(GameLibrary::KeyCode::Down))
    {
        m_menuIndex = (m_menuIndex + 1) % 3;
    }

    if (input.IsKeyPressed(GameLibrary::KeyCode::Enter) || input.IsKeyPressed(GameLibrary::KeyCode::Space))
    {
        switch (m_menuIndex)
        {
        case 0: // Resume
            scene.GetStateMachine().ChangeState<PlayingState>();
            break;
        case 1: // Settings
            scene.GetStateMachine().ChangeState<SettingsState>();
            break;
        case 2: // Quit
            scene.GetContainer().Resolve<GameLibrary::SceneManager>()->LoadScene("Title");
            break;
        default:
            break;
        }
    }
}