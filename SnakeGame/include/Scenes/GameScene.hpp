#pragma once

#include "Scene/Scene.hpp"

#include <memory>

namespace GameLibrary
{
    class FxSystem;
    class IInputProvider;
    class SoundSystem;
    class SaveSystem;
    struct EngineConfig;
} // namespace GameLibrary

namespace SnakeGame
{
    struct GameData;
}

class GameScene : public GameLibrary::Scene
{
public:
    GameScene(const std::string& name, GameLibrary::ServiceContainer& container);

    void OnEnter() override;
    void OnExit() override;
    void Update(float deltaTime) override;
    void Render(GameLibrary::IGraphics& graphics) override;

private:
    enum class PauseState
    {
        None,
        Menu,
        Settings
    };

    void ApplyConfig();
    void SetupGame();

    void UpdatePauseMenu();
    void UpdateSettings();

    void RenderPauseMenu(GameLibrary::IGraphics& graphics);
    void RenderSettings(GameLibrary::IGraphics& graphics);

    void SaveSettings();

    GameLibrary::FxSystem* m_fxSystem{};
    GameLibrary::IInputProvider* m_input{};
    GameLibrary::SoundSystem* m_soundSystem{};
    GameLibrary::SaveSystem* m_saveSystem{};
    GameLibrary::EngineConfig* m_engineConfig{};
    SnakeGame::GameData* m_gameData{};

    PauseState m_pauseState{PauseState::None};
    int32_t m_menuIndex{};
    int32_t m_settingsIndex{};
};