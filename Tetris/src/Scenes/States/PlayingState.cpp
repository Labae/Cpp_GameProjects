//
// Created by Labae on 3/14/26.
//

#include "Scenes/States/PlayingState.hpp"

#include "Interfaces/IInputProvider.hpp"
#include "Scenes/SingleGameScene.hpp"
#include "Scenes/States/PausedState.hpp"
#include "States/StateMachine.hpp"

namespace Tetris
{
    void PlayingState::Update(SingleGameScene& scene, [[maybe_unused]] const float deltaTime)
    {
        const auto& input = scene.GetInput();

        if (input.IsKeyPressed(GameLibrary::KeyCode::Escape))
        {
            scene.GetStateMachine().ChangeState<PausedState>();
            return;
        }

        scene.UpdateActors(deltaTime);

        if (input.IsKeyPressed(GameLibrary::KeyCode::C) || input.IsKeyPressed(GameLibrary::KeyCode::LShift))
        {
            scene.Hold();
        }
    }
} // namespace Tetris