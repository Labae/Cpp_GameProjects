//
// Created by Labae on 3/14/26.
//

#include "Scenes/States/PausedState.hpp"

#include "Core/ServiceContainer.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Scene/SceneManager.hpp"
#include "Scenes/SingleGameScene.hpp"
#include "Scenes/States/PlayingState.hpp"
#include "States/StateMachine.hpp"

namespace Tetris
{
    void PausedState::OnEnter([[maybe_unused]] SingleGameScene& scene)
    {
        m_menuIndex = 0;
    }

    void PausedState::Update(SingleGameScene& scene, [[maybe_unused]] const float deltaTime)
    {
        auto& input = scene.GetInput();

        if (input.IsKeyPressed(GameLibrary::KeyCode::Escape))
        {
            scene.GetStateMachine().ChangeState<PlayingState>();
            return;
        }

        if (input.IsKeyPressed(GameLibrary::KeyCode::Up))
        {
            m_menuIndex = (m_menuIndex + 1) % 2;
        }
        if (input.IsKeyPressed(GameLibrary::KeyCode::Down))
        {
            m_menuIndex = (m_menuIndex + 1) % 2;
        }

        if (input.IsKeyPressed(GameLibrary::KeyCode::Enter))
        {
            if (m_menuIndex == 0)
            {
                scene.GetStateMachine().ChangeState<PlayingState>();
            }
            else
            {
                scene.GetContainer().Resolve<GameLibrary::SceneManager>()->LoadScene("Title");
            }
        }
    }
} // namespace Tetris