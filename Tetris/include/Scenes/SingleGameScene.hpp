//
// Created by Labae on 3/1/26.
//

#ifndef SINGLEGAMESCENE_HPP
#define SINGLEGAMESCENE_HPP

#include "Components/ActivePiece.hpp"
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
        void Render(GameLibrary::IGraphics& graphics) override;

    private:
        void SpawnNewPiece();
        void Hold();
        void RenderHoldBox(GameLibrary::IGraphics& graphics) const;

        const TetrisConfig* m_tetrisConfig{};
        GameLibrary::IInputProvider* m_inputProvider{};
        ActivePiece* m_activePiece{};
        Board* m_board{};

        std::optional<ETetromino> m_holdPiece{};
        bool m_canHold{true};

        std::mt19937 m_rnd{std::random_device{}()};
    };
} // namespace Tetris

#endif // SINGLEGAMESCENE_HPP
