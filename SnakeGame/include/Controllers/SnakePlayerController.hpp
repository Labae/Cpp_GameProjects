//
// Created by Labae on 1/11/26.
//

#ifndef SNAKEPLAYERCONTROLLER_HPP
#define SNAKEPLAYERCONTROLLER_HPP

#include "Actor/Controller.hpp"
#include "Interfaces/IInputProvider.hpp"

namespace SnakeGame
{
    class SnakePlayerController final : public GameLibrary::Controller
    {
    public:
        explicit SnakePlayerController(GameLibrary::IInputProvider& input);

        void Update(float deltaTime) override;

    private:
        GameLibrary::IInputProvider& m_input;
    };
} // namespace SnakeGame

#endif // SNAKEPLAYERCONTROLLER_HPP
