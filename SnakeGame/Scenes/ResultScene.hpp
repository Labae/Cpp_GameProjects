#pragma once

#include "Scene/Scene.hpp"

namespace GameLibrary
{
    class IInputProvider;
    class ISceneManager;
    struct EngineConfig;
}

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
    GameLibrary::IInputProvider* m_input = nullptr;
    GameLibrary::ISceneManager* m_sceneManager = nullptr;
    const GameLibrary::EngineConfig* m_engineConfig = nullptr;
    SnakeGame::GameData* m_gameData = nullptr;

    bool m_isNewHighScore = false;
};