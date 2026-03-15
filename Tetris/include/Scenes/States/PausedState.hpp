//
// Created by Labae on 3/14/26.
//

#ifndef PAUSEDSTATE_HPP
#define PAUSEDSTATE_HPP

#include "States/IState.hpp"
#include "UI/MenuSelector.hpp"

#include <optional>

namespace Tetris
{
    class SingleGameScene;

    class PausedState final : public GameLibrary::IState<SingleGameScene>
    {
    public:
        void OnEnter(SingleGameScene& scene) override;
        void Update(SingleGameScene& scene, float deltaTime) override;
        void Render(SingleGameScene& scene, GameLibrary::IGraphics& graphics) override;

    private:
        std::optional<GameLibrary::MenuSelector> m_menu{};
    };
} // namespace Tetris


#endif //PAUSEDSTATE_HPP
