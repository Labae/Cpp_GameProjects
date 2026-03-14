//
// Created by Labae on 3/14/26.
//

#ifndef PLAYINGSTATE_HPP
#define PLAYINGSTATE_HPP

#include "States/IState.hpp"

namespace Tetris
{
    class SingleGameScene;

    class PlayingState final : public GameLibrary::IState<SingleGameScene>
    {
    public:
        void Update(SingleGameScene& scene, float deltaTime) override;
    };
} // namespace Tetris


#endif //PLAYINGSTATE_HPP
