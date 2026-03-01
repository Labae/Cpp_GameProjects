//
// Created by Labae on 3/1/26.
//

#ifndef SINGLEGAMESCENE_HPP
#define SINGLEGAMESCENE_HPP

#include "Configs/TetrisConfig.hpp"
#include "Scene/Scene.hpp"

namespace GameLibrary
{
    class IInputProvider;
}

namespace Tetris
{
    class SingleGameScene final : public GameLibrary::Scene
    {
    public:
        SingleGameScene(const std::string& name, GameLibrary::ServiceContainer& container);

        void OnEnter() override;

    private:
        const TetrisConfig* m_tetrisConfig{};
    };
} // namespace Tetris

#endif // SINGLEGAMESCENE_HPP
