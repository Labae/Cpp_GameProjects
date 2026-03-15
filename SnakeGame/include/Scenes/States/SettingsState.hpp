#ifndef SNAKE_SETTINGSSTATE_HPP
#define SNAKE_SETTINGSSTATE_HPP

#include "States/IState.hpp"
#include "UI/TextBox.hpp"

#include <cstdint>

class GameScene;

class SettingsState final : public GameLibrary::IState<GameScene>
{
public:
    void OnEnter(GameScene& scene) override;
    void Update(GameScene& scene, float deltaTime) override;
    void Render(GameScene& scene, GameLibrary::IGraphics& graphics) override;

private:
    int32_t m_settingsIndex{};
    GameLibrary::TextBox m_titleBox{};
};

#endif // SNAKE_SETTINGSSTATE_HPP