#pragma once

#include "Interfaces/IInitializable.hpp"
#include "Services/ResourceService.hpp"

#include <SFML/Audio.hpp>
#include <memory>
#include <string>

namespace GameLibrary
{
    class AudioService final : public IInitializable
    {
    public:
        AudioService() = default;
        ~AudioService() override = default;

        AudioService(const AudioService&) = delete;
        AudioService(AudioService&&) = delete;
        AudioService& operator=(const AudioService&) = delete;
        AudioService& operator=(AudioService&&) = delete;

        void Init(ServiceContainer& container) override;

        // 효과음 재생
        void Play(const std::string& name);

        // 모든 효과음 중지
        void StopAll();

        // 배경 음악 재생 (루프)
        [[nodiscard]] bool PlayBGM(const std::string& filePath);

        // 배경 음악 중지
        void StopBGM();

        // 볼륨 설정 (0.0 ~ 1.0)
        void SetBGMVolume(float volume);
        void SetSFXVolume(float volume);

        [[nodiscard]] float GetBGMVolume() const noexcept { return m_bgmVolume; }
        [[nodiscard]] float GetSFXVolume() const noexcept { return m_sfxVolume; }
        [[nodiscard]] bool IsBGMPlaying() const noexcept { return m_bgm.getStatus() == sf::Music::Status::Playing; }

    private:
        ResourceService* m_resourceService{};
        std::vector<std::unique_ptr<sf::Sound>> m_sounds{};

        sf::Music m_bgm{};

        float m_bgmVolume{1.0f};
        float m_sfxVolume{1.0f};
    };
} // namespace GameLibrary