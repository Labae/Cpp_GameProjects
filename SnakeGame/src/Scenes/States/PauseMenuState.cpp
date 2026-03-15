//
// Created by Labae on 3/14/26.
//

#include "Scenes/States/PauseMenuState.hpp"

#include "Core/EngineConfig.hpp"
#include "Core/ServiceContainer.hpp"
#include "Interfaces/IGraphics.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Scene/SceneManager.hpp"
#include "Scenes/GameScene.hpp"
#include "Scenes/States/PlayingState.hpp"
#include "Scenes/States/SettingsState.hpp"

void PauseMenuState::OnEnter(GameScene& scene)
{
    if (not m_menu.has_value())
    {
        m_menu.emplace(scene.GetInput());

        m_menu->SetItems({"Resume", "Settings", "Quit"});
        m_menu->SetStyle({
            .fontSize = 24,
            .selectedFontSize = 28,
            .spacing = 35,
            .normalColor = sf::Color(180, 180, 180, 255),
            .selectedColor = sf::Color(255, 255, 0, 255),
            .align = GameLibrary::TextAlign::Center,
        });
    }

    m_menu->Reset();
}

void PauseMenuState::Update(GameScene& scene, [[maybe_unused]] const float deltaTime)
{
    m_menu->Update();

    if (m_menu->IsCancelled())
    {
        scene.GetStateMachine().ChangeState<PlayingState>();
        return;
    }

    if (m_menu->IsConfirmed())
    {
        switch (m_menu->GetSelectedIndex())
        {
        case 0: // Resume
            scene.GetStateMachine().ChangeState<PlayingState>();
            break;
        case 1: // Settings
            scene.GetStateMachine().ChangeState<SettingsState>();
            break;
        case 2: // Quit
            scene.GetContainer().Resolve<GameLibrary::SceneManager>()->LoadScene("Title");
            break;
        default:
            break;
        }
    }
}

void PauseMenuState::Render(GameScene& scene, GameLibrary::IGraphics& graphics)
{
    const auto& config = scene.GetEngineConfig();
    const int32_t screenW = config.screenWidth;
    const int32_t screenH = config.screenHeight;

    // 반투명 오버레이
    graphics.FillRect(0, 0, screenW, screenH, sf::Color(0, 0, 0, 150));

    // 일시정지 박스
    constexpr int32_t boxW = 300;
    constexpr int32_t boxH = 200;
    const int32_t boxX = (screenW - boxW) / 2;
    const int32_t boxY = (screenH - boxH) / 2;

    graphics.FillRect(boxX, boxY, boxW, boxH, sf::Color(40, 40, 40, 255));
    graphics.DrawRect(boxX, boxY, boxW, boxH, sf::Color(255, 255, 255, 255));

    // 타이틀
    graphics.DrawLabel("PAUSED", boxX + boxW / 2, boxY + 20, 36, sf::Color(255, 255, 255, 255),
                       GameLibrary::TextAlign::Center);

    // 메뉴 항목
    m_menu->SetPosition(boxX + boxW / 2, boxY + 70);
    m_menu->Render(graphics);
}
