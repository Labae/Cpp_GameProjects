//
// Created by Labae on 3/14/26.
//

#include "Scenes/States/SettingsState.hpp"

#include "Core/EngineConfig.hpp"
#include "Interfaces/IGraphics.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Scenes/GameScene.hpp"
#include "Scenes/States/PauseMenuState.hpp"
#include "Services/AudioService.hpp"
#include "States/StateMachine.hpp"

void SettingsState::OnEnter([[maybe_unused]] GameScene& scene)
{
    m_settingsIndex = 0;
}

void SettingsState::Update(GameScene& scene, [[maybe_unused]] const float deltaTime)
{
    const auto& input = scene.GetInput();

    if (input.IsKeyPressed(GameLibrary::KeyCode::Escape))
    {
        scene.GetStateMachine().ChangeState<PauseMenuState>();
        return;
    }

    if (input.IsKeyPressed(GameLibrary::KeyCode::Up))
    {
        m_settingsIndex = (m_settingsIndex + 1) % 2;
    }
    if (input.IsKeyPressed(GameLibrary::KeyCode::Down))
    {
        m_settingsIndex = (m_settingsIndex + 1) % 2;
    }

    constexpr float delta = 0.1f;
    auto* audio = scene.GetAudioService();
    if (not audio)
    {
        return;
    }

    bool changed = false;

    if (input.IsKeyPressed(GameLibrary::KeyCode::Left))
    {
        if (m_settingsIndex == 0)
        {
            audio->SetBGMVolume(audio->GetBGMVolume() - delta);
        }
        else
        {
            audio->SetSFXVolume(audio->GetSFXVolume() - delta);
        }
        changed = true;
    }
    if (input.IsKeyPressed(GameLibrary::KeyCode::Right))
    {
        if (m_settingsIndex == 0)
        {
            audio->SetBGMVolume(audio->GetBGMVolume() + delta);
        }
        else
        {
            audio->SetSFXVolume(audio->GetSFXVolume() + delta);
        }
        changed = true;
    }

    if (changed)
    {
        scene.SaveSettings();
    }
}

void SettingsState::Render(GameScene& scene, GameLibrary::IGraphics& graphics)
{
    const auto& config = scene.GetEngineConfig();
    const int32_t screenW = config.screenWidth;
    const int32_t screenH = config.screenHeight;

    // 반투명 오버레이
    graphics.FillRect(0, 0, screenW, screenH, sf::Color(0, 0, 0, 150));

    // 설정 박스
    constexpr int32_t boxW = 350;
    constexpr int32_t boxH = 200;
    const int32_t boxX = (screenW - boxW) / 2;
    const int32_t boxY = (screenH - boxH) / 2;

    graphics.FillRect(boxX, boxY, boxW, boxH, sf::Color(40, 40, 40, 255));
    graphics.DrawRect(boxX, boxY, boxW, boxH, sf::Color(255, 255, 255, 255));

    // 타이틀
    graphics.DrawLabel("SETTINGS", boxX + boxW / 2, boxY + 20, 36, sf::Color(255, 255, 255, 255),
                       GameLibrary::TextAlign::Center);

    // 볼륨 바
    auto* audio = scene.GetAudioService();
    const float bgmVol = audio ? audio->GetBGMVolume() : 1.0f;
    const float sfxVol = audio ? audio->GetSFXVolume() : 1.0f;

    const int32_t barX = boxX + 150;
    constexpr int32_t barW = 150;
    constexpr int32_t barH = 20;

    // BGM
    const sf::Color bgmColor = (m_settingsIndex == 0) ? sf::Color(255, 255, 0, 255) : sf::Color(180, 180, 180, 255);
    graphics.DrawLabel("BGM", boxX + 30, boxY + 70, 24, bgmColor, GameLibrary::TextAlign::Left);
    graphics.DrawRect(barX, boxY + 70, barW, barH, sf::Color(100, 100, 100, 255));
    graphics.FillRect(barX, boxY + 70, static_cast<int32_t>(barW * bgmVol), barH, sf::Color(0, 200, 0, 255));

    // SFX
    const sf::Color sfxColor = (m_settingsIndex == 1) ? sf::Color(255, 255, 0, 255) : sf::Color(180, 180, 180, 255);
    graphics.DrawLabel("SFX", boxX + 30, boxY + 110, 24, sfxColor, GameLibrary::TextAlign::Left);
    graphics.DrawRect(barX, boxY + 110, barW, barH, sf::Color(100, 100, 100, 255));
    graphics.FillRect(barX, boxY + 110, static_cast<int32_t>(barW * sfxVol), barH, sf::Color(0, 200, 0, 255));

    // 안내
    graphics.DrawLabel("< > to adjust, ESC to back", boxX + boxW / 2, boxY + 160, 18, sf::Color(120, 120, 120, 255),
                       GameLibrary::TextAlign::Center);
}