//
// Created by Labae on 3/14/26.
//

#include "Scenes/States/GameOverState.hpp"

#include "Scenes/SingleGameScene.hpp"
#include "Core/ServiceContainer.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Scene/SceneManager.hpp"

namespace Tetris
{
    void GameOverState::Update(SingleGameScene& scene, [[maybe_unused]] const float deltaTime)
    {
        if (scene.GetInput().IsKeyPressed(GameLibrary::KeyCode::Escape))
        {
            scene.GetContainer().Resolve<GameLibrary::SceneManager>()->LoadScene("Title");
        }
    }
} // namespace Tetris