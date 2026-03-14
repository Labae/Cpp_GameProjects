#ifndef SNAKE_PAUSEMENUSTATE_HPP
#define SNAKE_PAUSEMENUSTATE_HPP

#include "States/IState.hpp"

#include <cstdint>

class GameScene;

class PauseMenuState final : public GameLibrary::IState<GameScene>
{
public:
    void OnEnter(GameScene& scene) override;
    void Update(GameScene& scene, float deltaTime) override;

    [[nodiscard]] int32_t GetMenuIndex() const noexcept { return m_menuIndex; }

private:
    int32_t m_menuIndex{};
};

#endif // SNAKE_PAUSEMENUSTATE_HPP