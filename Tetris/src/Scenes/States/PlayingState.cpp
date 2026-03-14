//
// Created by Labae on 3/14/26.
//

#include "Scenes/States/PlayingState.hpp"

#include "Input/InputActionMap.hpp"
#include "Scenes/SingleGameScene.hpp"
#include "Scenes/States/PausedState.hpp"
#include "States/StateMachine.hpp"

namespace Tetris
{
    void PlayingState::Update(SingleGameScene& scene, [[maybe_unused]] const float deltaTime)
    {
        auto& actionMap = scene.GetActionMap();

        if (actionMap.IsPressed("Pause"))
        {
            scene.GetStateMachine().ChangeState<PausedState>();
            return;
        }

        scene.UpdateActors(deltaTime);

        if (actionMap.IsPressed("Hold"))
        {
            scene.Hold();
        }
    }
} // namespace Tetris