#ifndef SNAKE_SETTINGSSTATE_HPP
#define SNAKE_SETTINGSSTATE_HPP

#include "States/IState.hpp"

#include <cstdint>

class GameScene;

class SettingsState final : public GameLibrary::IState<GameScene>
{
public:
    void OnEnter(GameScene& scene) override;
    void Update(GameScene& scene, float deltaTime) override;

    [[nodiscard]] int32_t GetSettingsIndex() const noexcept { return m_settingsIndex; }

private:
    int32_t m_settingsIndex{};
};

#endif // SNAKE_SETTINGSSTATE_HPP