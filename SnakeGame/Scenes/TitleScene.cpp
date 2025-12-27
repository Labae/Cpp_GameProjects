#include "TitleScene.hpp"
#include "Core/ServiceContainer.hpp"
#include "Core/EngineConfig.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Interfaces/ISceneManager.hpp"
#include "Interfaces/IGraphics.hpp"

TitleScene::TitleScene(const std::string& name, GameLibrary::ServiceContainer& container)
    : Scene(name, container)
{
}

void TitleScene::OnEnter()
{
    auto& container = GetContainer();
    m_input = container.Resolve<GameLibrary::IInputProvider>();
    m_sceneManager = container.Resolve<GameLibrary::ISceneManager>();
    m_engineConfig = container.Resolve<GameLibrary::EngineConfig>();
}

void TitleScene::Update(float deltaTime)
{
    // 텍스트 깜빡임
    m_blinkTimer += deltaTime;
    if (m_blinkTimer >= 0.5f)
    {
        m_blinkTimer = 0.0f;
        m_showText = !m_showText;
    }

    // SPACE 키로 게임 시작
    if (m_input->IsKeyPressed(GameLibrary::KeyCode::Space))
    {
        m_sceneManager->LoadScene("Game");
    }
}

void TitleScene::Render(GameLibrary::IGraphics& graphics)
{
    int screenWidth = m_engineConfig->screenWidth;
    int centerX = 0;

    // 타이틀
    graphics.DrawLabel(L"SNAKE GAME", centerX, 250, screenWidth, 72, { 0, 255, 0, 255 }, GameLibrary::TextAlign::Center);

    // 부제
    graphics.DrawLabel(L"A Classic Arcade Game", centerX, 330, screenWidth, 24, { 150, 150, 150, 255 }, GameLibrary::TextAlign::Center);

    // 시작 안내 (깜빡임)
    if (m_showText)
    {
        graphics.DrawLabel(L"Press SPACE to Start", centerX, 500, screenWidth, 28, { 255, 255, 255, 255 }, GameLibrary::TextAlign::Center);
    }

    // 조작법
    graphics.DrawLabel(L"Use Arrow Keys to Move", centerX, 600, screenWidth, 18, { 100, 100, 100, 255 }, GameLibrary::TextAlign::Center);
}