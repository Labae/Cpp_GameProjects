#pragma once

#include <string>
#include <unordered_map>
#include <Windows.h>

namespace GameLibrary
{
    class SoundSystem
    {
    public:
        SoundSystem() = default;
        ~SoundSystem();

        // 효과음 로드 (이름으로 등록)
        void Load(const std::string& name, const std::wstring& filePath);

        // 효과음 재생
        void Play(const std::string& name);

        // 모든 효과음 중지
        void StopAll();

        // 배경 음악 재생 (루프)
        void PlayBGM(const std::wstring& filePath);

        // 배경 음악 중지
        void StopBGM();

        // 볼륨 설정 (0.0 ~ 1.0)
        void SetBGMVolume(float volume);
        void SetSFXVolume(float volume);

        [[nodiscard]] float GetBGMVolume() const noexcept { return m_bgmVolume; }
        [[nodiscard]] float GetSFXVolume() const noexcept { return m_sfxVolume; }

    private:
        void CloseAlias(const std::wstring& alias);

        std::unordered_map<std::string, std::wstring> m_sounds;        // name -> filePath
        std::unordered_map<std::string, std::wstring> m_activeAliases; // name -> alias
        bool m_bgmPlaying = false;
        float m_bgmVolume = 1.0f;
        float m_sfxVolume = 1.0f;
        int32_t m_aliasCounter = 0;
    };
}