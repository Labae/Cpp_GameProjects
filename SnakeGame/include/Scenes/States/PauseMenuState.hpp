#ifndef SNAKE_PAUSEMENUSTATE_HPP
#define SNAKE_PAUSEMENUSTATE_HPP

#include "States/IState.hpp"
#include "UI/MenuSelector.hpp"

#include <optional>

class GameScene;

class PauseMenuState final : public GameLibrary::IState<GameScene>
{
public:
    void OnEnter(GameScene& scene) override;
    void Update(GameScene& scene, float deltaTime) override;
    void Render(GameScene& scene, GameLibrary::IGraphics& graphics) override;

private:
    std::optional<GameLibrary::MenuSelector> m_menu{};
};

#endif // SNAKE_PAUSEMENUSTATE_HPP