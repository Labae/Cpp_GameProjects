#pragma once

#include "Scene/Scene.hpp"

namespace GameLibrary
{
    class IInputProvider;
    class ISceneManager;
    struct EngineConfig;
} // namespace GameLibrary

namespace SnakeGame
{
    struct GameData;
}

class ResultScene : public GameLibrary::Scene
{
public:
    ResultScene(const std::string& name, GameLibrary::ServiceContainer& container);

    void OnEnter() override;
    void Update(float deltaTime) override;
    void Render(GameLibrary::IGraphics& graphics) override;

private:
    GameLibrary::IInputProvider* m_input{};
    GameLibrary::ISceneManager* m_sceneManager{};
    const GameLibrary::EngineConfig* m_engineConfig{};
    SnakeGame::GameData* m_gameData{};

    bool m_isNewHighScore{};
};