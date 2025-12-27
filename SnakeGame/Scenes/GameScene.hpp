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
}

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

    std::unique_ptr<GameLibrary::FxSystem> m_fxSystem;
    GameLibrary::IInputProvider* m_input = nullptr;
    GameLibrary::SoundSystem* m_soundSystem = nullptr;
    GameLibrary::SaveSystem* m_saveSystem = nullptr;
    GameLibrary::EngineConfig* m_engineConfig = nullptr;
    SnakeGame::GameData* m_gameData = nullptr;

    PauseState m_pauseState = PauseState::None;
    int m_menuIndex = 0;
    int m_settingsIndex = 0;
};