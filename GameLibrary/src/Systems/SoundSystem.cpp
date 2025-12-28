#include "Systems/SoundSystem.hpp"

#include "Systems/Logger.hpp"

namespace GameLibrary
{
    bool SoundSystem::Load(const std::string& name, const std::string& filePath)
    {
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(filePath))
        {
            Logger::Warning("Failed to load sound from file: " + filePath);
            return false;
        }

        m_buffers[name] = std::move(buffer);
        return true;
    }

    void SoundSystem::Play(const std::string& name)
    {
        if (m_sfxVolume <= 0.0f)
        {
            return;
        }

        auto it = m_buffers.find(name);
        if (it == m_buffers.end())
        {
            Logger::Warning("Failed to find sound: " + name);
            return;
        }

        m_sounds.erase(std::remove_if(m_sounds.begin(), m_sounds.end(), [](const auto& sound)
                                      { return sound->getStatus() == sf::Sound::Status::Stopped; }),
                       m_sounds.end());

        auto sound = std::make_unique<sf::Sound>(it->second);
        sound->setVolume(m_sfxVolume * 100.0f);
        sound->play();

        m_sounds.emplace_back(std::move(sound));
    }

    void SoundSystem::StopAll()
    {
        for (auto& sound : m_sounds)
        {
            sound->stop();
        }
        m_sounds.clear();
    }

    bool SoundSystem::PlayBGM(const std::string& filePath)
    {
        StopBGM();

        if (!m_bgm.openFromFile(filePath))
        {
            Logger::Warning("Failed to open BGM from file: " + filePath);
            return false;
        }

        m_bgm.setLooping(true);
        m_bgm.setVolume(m_bgmVolume);
        m_bgm.play();

        return true;
    }

    void SoundSystem::StopBGM()
    {
        if (IsBGMPlaying())
        {
            m_bgm.stop();
        }
    }

    void SoundSystem::SetBGMVolume(float volume)
    {
        m_bgmVolume = std::clamp(volume, 0.0f, 1.0f);

        if (IsBGMPlaying())
        {
            m_bgm.setVolume(m_bgmVolume * 100.0f);
        }
    }

    void SoundSystem::SetSFXVolume(float volume)
    {
        m_sfxVolume = std::clamp(volume, 0.0f, 1.0f);
    }
} // namespace GameLibrary