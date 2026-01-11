//
// Created by Labae on 1/12/26.
//

#include "Scenes/TitleScene.hpp"

#include "Core/EngineConfig.hpp"
#include "Core/ServiceContainer.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Scene/SceneManager.hpp"

namespace Tetris
{
    namespace
    {
        // 레이아웃
        constexpr int32_t TITLE_Y = 150;
        constexpr int32_t MENU_START_Y = 350;
        constexpr int32_t MENU_SPACING = 60;
        constexpr int32_t CURSOR_OFFSET_X = 140;
        constexpr int32_t HINT_Y = 700;

        // 폰트 크기
        constexpr int32_t TITLE_FONT_SIZE = 80;
        constexpr int32_t MENU_FONT_SIZE = 32;
        constexpr int32_t MENU_FONT_SIZE_SELECTED = 36;
        constexpr int32_t HINT_FONT_SIZE = 18;

        // 타이밍
        constexpr float CURSOR_BLINK_INTERVAL = 0.4f;

        // 색상
        constexpr sf::Color COLOR_TITLE = sf::Color::Cyan;
        constexpr sf::Color COLOR_MENU_SELECTED = sf::Color::Yellow;
        constexpr sf::Color COLOR_MENU_NORMAL{150, 150, 150, 255};
        constexpr sf::Color COLOR_HINT{100, 100, 100, 255};

        // 타이틀 텍스트
        constexpr auto TITLE_TEXT = "TETRIS";

        // 메뉴 텍스트
        constexpr const char* MENU_LABELS[] = {"SINGLE PLAY", "BATTLE MODE", "SETTINGS", "EXIT"};
    } // namespace

    TitleScene::TitleScene(const std::string& name, GameLibrary::ServiceContainer& container) : Scene(name, container)
    {
    }

    void TitleScene::OnEnter()
    {
        auto& container = GetContainer();
        m_input = container.Resolve<GameLibrary::IInputProvider>();
        m_sceneManager = container.Resolve<GameLibrary::SceneManager>();
        m_engineConfig = container.Resolve<GameLibrary::EngineConfig>();

        m_selectedMenu = EMenuItem::SinglePlay;
    }

    void TitleScene::Update(const float deltaTime)
    {
        // 커서 깜빡임
        m_blinkTimer += deltaTime;
        if (m_blinkTimer >= CURSOR_BLINK_INTERVAL)
        {
            m_blinkTimer = 0.0f;
            m_showCursor = !m_showCursor;
        }

        // 메뉴 이동
        if (m_input->IsKeyPressed(GameLibrary::KeyCode::Up))
        {
            MenuUp();
        }
        if (m_input->IsKeyPressed(GameLibrary::KeyCode::Down))
        {
            MenuDown();
        }

        // 선택
        if (m_input->IsKeyPressed(GameLibrary::KeyCode::Enter) or m_input->IsKeyPressed(GameLibrary::KeyCode::Space))
        {
            SelectMenuItem();
        }
    }

    void TitleScene::Render(GameLibrary::IGraphics& graphics)
    {
        const int32_t centerX = m_engineConfig->screenWidth / 2;

        // 타이틀
        graphics.DrawLabel(TITLE_TEXT, centerX, TITLE_Y, TITLE_FONT_SIZE, COLOR_TITLE, GameLibrary::TextAlign::Center);

        // 메뉴 항목
        constexpr auto menuCount = static_cast<int32_t>(EMenuItem::Count);
        for (int32_t i = 0; i < menuCount; ++i)
        {
            const bool isSelected = (i == static_cast<int32_t>(m_selectedMenu));
            const sf::Color color = isSelected ? COLOR_MENU_SELECTED : COLOR_MENU_NORMAL;
            const int32_t fontSize = isSelected ? MENU_FONT_SIZE_SELECTED : MENU_FONT_SIZE;
            const int32_t y = MENU_START_Y + i * MENU_SPACING;

            graphics.DrawLabel(MENU_LABELS[i], centerX, y, fontSize, color, GameLibrary::TextAlign::Center);
        }

        // 선택 커서
        if (m_showCursor)
        {
            const int32_t cursorY = MENU_START_Y + static_cast<int32_t>(m_selectedMenu) * MENU_SPACING;
            graphics.DrawLabel(">", centerX - CURSOR_OFFSET_X, cursorY, MENU_FONT_SIZE_SELECTED, COLOR_MENU_SELECTED,
                               GameLibrary::TextAlign::Center);
            graphics.DrawLabel("<", centerX + CURSOR_OFFSET_X, cursorY, MENU_FONT_SIZE_SELECTED, COLOR_MENU_SELECTED,
                               GameLibrary::TextAlign::Center);
        }

        // 하단 안내
        graphics.DrawLabel("Use Arrow Keys to Navigate, Enter to Select", centerX, HINT_Y, HINT_FONT_SIZE, COLOR_HINT,
                           GameLibrary::TextAlign::Center);
    }

    void TitleScene::SelectMenuItem() const
    {
        switch (m_selectedMenu)
        {
        case EMenuItem::SinglePlay:
        {
            m_sceneManager->LoadScene("SingleGame");
            break;
        }
        case EMenuItem::BattleMode:
        {
            m_sceneManager->LoadScene("BattleLobby");
            break;
        }
        case EMenuItem::Settings:
        {
            // TODO: Settings Scene
            break;
        }
        case EMenuItem::Exit:
        {
            // TODO: Engine Quit
            break;
        }
        case EMenuItem::Count:
        default:;
        }
    }

    void TitleScene::MenuUp()
    {
        constexpr auto count = static_cast<int32_t>(EMenuItem::Count);
        auto index = static_cast<int32_t>(m_selectedMenu);
        index = (index - 1 + count) % count;
        m_selectedMenu = static_cast<EMenuItem>(index);
    }

    void TitleScene::MenuDown()
    {
        constexpr auto count = static_cast<int32_t>(EMenuItem::Count);
        auto index = static_cast<int32_t>(m_selectedMenu);
        index = (index + 1) % count;
        m_selectedMenu = static_cast<EMenuItem>(index);
    }

} // namespace Tetris