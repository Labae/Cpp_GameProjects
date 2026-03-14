#ifndef SNAKE_PLAYINGSTATE_HPP
#define SNAKE_PLAYINGSTATE_HPP

#include "States/IState.hpp"

class GameScene;

class PlayingState final : public GameLibrary::IState<GameScene>
{
public:
    void Update(GameScene& scene, float deltaTime) override;
};

#endif // SNAKE_PLAYINGSTATE_HPP