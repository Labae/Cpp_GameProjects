//
// Created by Labae on 3/14/26.
//

#include "Scenes/States/PlayingState.hpp"

#include "Scenes/GameScene.hpp"
#include "Scenes/States/PauseMenuState.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "States/StateMachine.hpp"

void PlayingState::Update(GameScene& scene, const float deltaTime)
{
    if (scene.GetInput().IsKeyPressed(GameLibrary::KeyCode::Escape))
    {
        scene.GetStateMachine().ChangeState<PauseMenuState>();
        return;
    }

    scene.UpdateActors(deltaTime);
}
