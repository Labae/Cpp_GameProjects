#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include "Scene/Scene.hpp"
#include "States/StateMachine.hpp"

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
    class SnakePlayerController;
} // namespace SnakeGame

class GameScene final : public GameLibrary::Scene
{
public:
    GameScene(const std::string& name, GameLibrary::ServiceContainer& container);
    ~GameScene() override;

    void OnEnter() override;
    void OnExit() override;
    void Update(float deltaTime) override;
    void Render(GameLibrary::IGraphics& graphics) override;

    // State에서 접근하는 public 인터페이스
    [[nodiscard]] GameLibrary::IInputProvider& GetInput() const noexcept { return *m_input; }
    [[nodiscard]] GameLibrary::StateMachine<GameScene>& GetStateMachine() noexcept { return m_stateMachine; }
    [[nodiscard]] const GameLibrary::EngineConfig& GetEngineConfig() const noexcept { return *m_engineConfig; }
    [[nodiscard]] GameLibrary::AudioService* GetAudioService() const noexcept { return m_audioService; }

    void UpdateActors(float deltaTime);
    void SaveSettings() const;

private:
    void ApplyConfig() const;
    void SetupGame();

    void RenderPauseMenu(GameLibrary::IGraphics& graphics) const;
    void RenderSettings(GameLibrary::IGraphics& graphics) const;

    GameLibrary::StateMachine<GameScene> m_stateMachine{*this};

    GameLibrary::FxService* m_fxSystem{};
    GameLibrary::IInputProvider* m_input{};
    GameLibrary::AudioService* m_audioService{};
    GameLibrary::SaveService* m_saveSystem{};
    GameLibrary::EngineConfig* m_engineConfig{};
    SnakeGame::GameData* m_gameData{};

    std::unique_ptr<SnakeGame::SnakePlayerController> m_playerController{};
};

#endif // GAMESCENE_HPP