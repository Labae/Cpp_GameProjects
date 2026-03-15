//
// Created by Labae on 3/14/26.
//

#include "Scenes/States/PausedState.hpp"

#include "Core/EngineConfig.hpp"
#include "Core/ServiceContainer.hpp"
#include "Interfaces/IGraphics.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Scene/SceneManager.hpp"
#include "Scenes/SingleGameScene.hpp"
#include "Scenes/States/PlayingState.hpp"
#include "States/StateMachine.hpp"

namespace Tetris
{
    void PausedState::OnEnter(SingleGameScene& scene)
    {
        if (not m_menu.has_value())
        {
            m_menu.emplace(scene.GetInput());

            m_menu->SetItems({"Resume", "Title"});
            m_menu->SetStyle({
                .fontSize = 24,
                .selectedFontSize = 28,
                .spacing = 40,
                .normalColor = sf::Color(150, 150, 150, 255),
                .selectedColor = sf::Color::Yellow,
                .align = GameLibrary::TextAlign::Center,
            });
        }

        m_menu->Reset();
    }

    void PausedState::Update(SingleGameScene& scene, [[maybe_unused]] const float deltaTime)
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
            case 1: // Title
                scene.GetContainer().Resolve<GameLibrary::SceneManager>()->LoadScene("Title");
                break;
            default:
                break;
            }
        }
    }

    void PausedState::Render(SingleGameScene& scene, GameLibrary::IGraphics& graphics)
    {
        const auto& config = scene.GetEngineConfig();
        const int32_t screenW = config.screenWidth;
        const int32_t screenH = config.screenHeight;
        const int32_t centerX = screenW / 2;
        const int32_t centerY = screenH / 2;

        graphics.FillRect(0, 0, screenW, screenH, sf::Color(0, 0, 0, 180));

        graphics.DrawLabel("PAUSED", centerX, centerY - 80, 48, sf::Color::White, GameLibrary::TextAlign::Center);

        m_menu->SetPosition(centerX, centerY);
        m_menu->Render(graphics);
    }
} // namespace Tetris
