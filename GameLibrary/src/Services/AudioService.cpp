#include "../../include/Services/AudioService.hpp"

#include "../../include/Systems/Logger.hpp"
#include "Core/ServiceContainer.hpp"

namespace GameLibrary
{
    void AudioService::Init(ServiceContainer& container)
    {
        m_resourceService = container.Resolve<ResourceService>();
    }

    void AudioService::Play(const std::string& name)
    {
        if (m_sfxVolume <= 0.0f)
        {
            return;
        }

        auto& buffer = m_resourceService->GetSound(name);
        std::erase_if(m_sounds,
            [](const auto& sound)
            {
                return sound->getStatus() == sf::Sound::Status::Stopped;
            });

        auto sound = std::make_unique<sf::Sound>(buffer);
        sound->setVolume(m_sfxVolume * 100.0f);
        sound->play();

        m_sounds.emplace_back(std::move(sound));
    }

    void AudioService::StopAll()
    {
        for (const auto& sound : m_sounds)
        {
            sound->stop();
        }
        m_sounds.clear();
    }

    bool AudioService::PlayBGM(const std::string& filePath)
    {
        StopBGM();

        if (!m_bgm.openFromFile(std::string(filePath)))
        {
            Logger::Warning("Failed to open BGM from file: " + std::string(filePath));
            return false;
        }

        m_bgm.setLooping(true);
        m_bgm.setVolume(m_bgmVolume);
        m_bgm.play();

        return true;
    }

    void AudioService::StopBGM()
    {
        if (IsBGMPlaying())
        {
            m_bgm.stop();
        }
    }

    void AudioService::SetBGMVolume(const float volume)
    {
        m_bgmVolume = std::clamp(volume, 0.0f, 1.0f);

        if (IsBGMPlaying())
        {
            m_bgm.setVolume(m_bgmVolume * 100.0f);
        }
    }

    void AudioService::SetSFXVolume(const float volume)
    {
        m_sfxVolume = std::clamp(volume, 0.0f, 1.0f);
    }
} // namespace GameLibrary