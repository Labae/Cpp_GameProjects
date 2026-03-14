//
// Created by Labae on 3/14/26.
//

#ifndef PAUSEDSTATE_HPP
#define PAUSEDSTATE_HPP

#include "States/IState.hpp"

#include <cstdint>

namespace Tetris
{
    class SingleGameScene;

    class PausedState final : public GameLibrary::IState<SingleGameScene>
    {
    public:
        void OnEnter(SingleGameScene& scene) override;
        void Update(SingleGameScene& scene, float deltaTime) override;

        [[nodiscard]] int32_t GetMenuIndex() const noexcept { return m_menuIndex; }

    private:
        int32_t m_menuIndex{};
    };
} // namespace Tetris


#endif //PAUSEDSTATE_HPP
