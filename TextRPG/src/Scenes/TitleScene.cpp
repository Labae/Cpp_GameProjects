//
// Created by Labae on 3/15/26.
//

#include "Scenes/TitleScene.hpp"

#include "Core/ServiceContainer.hpp"
#include "Events/GameEvents.hpp"

namespace
{
    constexpr int32_t TITLE_Y = 150;
    constexpr int32_t TITLE_FONT_SIZE = 56;
    constexpr int32_t MENU_Y = 350;

    constexpr auto COLOR_TITLE = sf::Color(255, 215, 0, 255);
    constexpr auto COLOR_HINT = sf::Color(100, 100, 100, 255);
} // namespace

namespace TextRPG
{
    TitleScene::TitleScene(const std::string& name, GameLibrary::ServiceContainer& container) : Scene(name, container)
    {
    }

    void TitleScene::OnEnter()
    {
        auto& container = GetContainer();
        m_input = container.Resolve<GameLibrary::IInputProvider>();
        m_sceneManager = container.Resolve<GameLibrary::SceneManager>();
        m_eventService = container.Resolve<GameLibrary::EventService>();
        m_engineConfig = container.Resolve<GameLibrary::EngineConfig>();

        m_menu = std::make_unique<GameLibrary::MenuSelector>(*m_input);
        m_menu->SetItems({"Start", "Exit"});

        const int32_t centerX = m_engineConfig->screenWidth / 2;
        m_menu->SetPosition(centerX, MENU_Y);
        m_menu->SetStyle(
            {.fontSize = 28, .selectedFontSize = 32, .spacing = 50, .align = GameLibrary::TextAlign::Center});
    }

    void TitleScene::Update([[maybe_unused]] float deltaTime)
    {
        m_menu->Update();

        if (m_menu->IsConfirmed())
        {
            switch (m_menu->GetSelectedIndex())
            {
            case 0:
                m_sceneManager->LoadScene("JobSelect");
                break;
            case 1:
                m_eventService->Publish(QuitEvent{});
                break;
            default:
                break;
            }
        }
    }

    void TitleScene::Render(GameLibrary::IGraphics& graphics)
    {
        const int32_t centerX = m_engineConfig->screenWidth / 2;

        graphics.DrawLabel("DUNGEON DESCENT", centerX, TITLE_Y, TITLE_FONT_SIZE, COLOR_TITLE, GameLibrary::TextAlign::Center);

        m_menu->Render(graphics);

        graphics.DrawLabel("Arrow Keys to Navigate, Enter to Select", centerX, 550, 16, COLOR_HINT, GameLibrary::TextAlign::Center);
    }

} // namespace TextRPG