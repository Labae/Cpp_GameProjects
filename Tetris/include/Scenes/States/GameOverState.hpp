//
// Created by Labae on 3/14/26.
//

#ifndef GAMEOVERSTATE_HPP
#define GAMEOVERSTATE_HPP

#include "States/IState.hpp"

namespace Tetris
{
    class SingleGameScene;

    class GameOverState final : public GameLibrary::IState<SingleGameScene>
    {
    public:
        void Update(SingleGameScene& scene, float deltaTime) override;
    };
} // namespace Tetris


#endif //GAMEOVERSTATE_HPP
