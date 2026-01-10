#pragma once

#include "Scene/Scene.hpp"

#include <memory>

namespace GameLibrary
{
    class FxService;
    class IInputProvider;
    class AudioService;
    class SaveService;
    struct EngineConfig;
} // namespace GameLibrary

namespace SnakeGame
{
    struct GameData;
}

class GameScene final : public GameLibrary::Scene
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

    void RenderPauseMenu(GameLibrary::IGraphics& graphics) const;
    void RenderSettings(GameLibrary::IGraphics& graphics) const;

    void SaveSettings() const;

    GameLibrary::FxService* m_fxSystem{};
    GameLibrary::IInputProvider* m_input{};
    GameLibrary::AudioService* m_audioService{};
    GameLibrary::SaveService* m_saveSystem{};
    GameLibrary::EngineConfig* m_engineConfig{};
    SnakeGame::GameData* m_gameData{};

    PauseState m_pauseState{PauseState::None};
    int32_t m_menuIndex{};
    int32_t m_settingsIndex{};
};