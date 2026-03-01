//
// Created by Labae on 3/1/26.
//

#ifndef SINGLEGAMESCENE_HPP
#define SINGLEGAMESCENE_HPP

#include "Components/ActivePiece.hpp"
#include "Configs/TetrisConfig.hpp"
#include "Scene/Scene.hpp"

#include <random>

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
        void Update(float deltaTime) override;

    private:
        void SpawnNewPiece();

        const TetrisConfig* m_tetrisConfig{};
        ActivePiece* m_activePiece{};

        std::mt19937 m_rnd{std::random_device{}()};
    };
} // namespace Tetris

#endif // SINGLEGAMESCENE_HPP
