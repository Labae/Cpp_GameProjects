#pragma once

#include "Scene/Scene.hpp"

namespace GameLibrary
{
    class IInputProvider;
    class SceneManager;
    struct EngineConfig;
} // namespace GameLibrary

namespace SnakeGame
{
    struct GameData;
}

class ResultScene final : public GameLibrary::Scene
{
public:
    ResultScene(const std::string& name, GameLibrary::ServiceContainer& container);

    void OnEnter() override;
    void Update(float deltaTime) override;
    void Render(GameLibrary::IGraphics& graphics) override;

private:
    GameLibrary::IInputProvider* m_input{};
    GameLibrary::SceneManager* m_sceneManager{};
    const GameLibrary::EngineConfig* m_engineConfig{};
    SnakeGame::GameData* m_gameData{};

    bool m_isNewHighScore{};
};