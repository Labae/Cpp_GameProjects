#include "Scenes/TitleScene.hpp"

#include "Core/EngineConfig.hpp"
#include "Core/ServiceContainer.hpp"
#include "Interfaces/IGraphics.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Scene/SceneManager.hpp"

TitleScene::TitleScene(const std::string& name, GameLibrary::ServiceContainer& container) : Scene(name, container) {}

void TitleScene::OnEnter()
{
    auto& container = GetContainer();
    m_input = container.Resolve<GameLibrary::IInputProvider>();
    m_sceneManager = container.Resolve<GameLibrary::SceneManager>();
    m_engineConfig = container.Resolve<GameLibrary::EngineConfig>();
}

void TitleScene::Update(const float deltaTime)
{
    m_blinkTimer += deltaTime;
    if (m_blinkTimer >= 0.5f)
    {
        m_blinkTimer = 0.0f;
        m_showText = !m_showText;
    }

    if (m_input->IsKeyPressed(GameLibrary::KeyCode::Space))
    {
        m_sceneManager->LoadScene("Game");
    }
}

void TitleScene::Render(GameLibrary::IGraphics& graphics)
{
    const int32_t centerX = m_engineConfig->screenWidth / 2;

    // 타이틀
    graphics.DrawLabel("SNAKE GAME", centerX, 250, 72, sf::Color(0, 255, 0, 255), GameLibrary::TextAlign::Center);

    // 부제
    graphics.DrawLabel("A Classic Arcade Game", centerX, 330, 24, sf::Color(150, 150, 150, 255),
                       GameLibrary::TextAlign::Center);

    // 시작 안내 (깜빡임)
    if (m_showText)
    {
        graphics.DrawLabel("Press SPACE to Start", centerX, 500, 28, sf::Color(255, 255, 255, 255),
                           GameLibrary::TextAlign::Center);
    }

    // 조작법
    graphics.DrawLabel("Use Arrow Keys to Move", centerX, 600, 18, sf::Color(100, 100, 100, 255),
                       GameLibrary::TextAlign::Center);
}