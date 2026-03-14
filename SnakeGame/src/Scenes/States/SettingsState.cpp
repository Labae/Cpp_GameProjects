//
// Created by Labae on 3/14/26.
//

#include "Scenes/States/SettingsState.hpp"

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