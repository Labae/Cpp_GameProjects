#pragma once

#include "Scene/Scene.hpp"

namespace GameLibrary
{
    class IInputProvider;
    struct EngineConfig;
    class SceneManager;
} // namespace GameLibrary

class TitleScene final : public GameLibrary::Scene
{
public:
    TitleScene(const std::string& name, GameLibrary::ServiceContainer& container);

    void OnEnter() override;
    void Update(float deltaTime) override;
    void Render(GameLibrary::IGraphics& graphics) override;

private:
    GameLibrary::IInputProvider* m_input{};
    GameLibrary::SceneManager* m_sceneManager{};
    const GameLibrary::EngineConfig* m_engineConfig{};

    float m_blinkTimer{};
    bool m_showText{true};
};