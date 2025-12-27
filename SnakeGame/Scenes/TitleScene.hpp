#pragma once

#include "Scene/Scene.hpp"

namespace GameLibrary
{
    class IInputProvider;
    class ISceneManager;
    struct EngineConfig;
}

class TitleScene : public GameLibrary::Scene
{
public:
    TitleScene(const std::string& name, GameLibrary::ServiceContainer& container);

    void OnEnter() override;
    void Update(float deltaTime) override;
    void Render(GameLibrary::IGraphics& graphics) override;

private:
    GameLibrary::IInputProvider* m_input = nullptr;
    GameLibrary::ISceneManager* m_sceneManager = nullptr;
    const GameLibrary::EngineConfig* m_engineConfig = nullptr;

    float m_blinkTimer = 0.0f;
    bool m_showText = true;
};