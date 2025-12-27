#include "SoundSystem.hpp"
#include "Logger.hpp"
#include <mmsystem.h>
#include <format>
#include <algorithm>

#pragma comment(lib, "winmm.lib")

namespace GameLibrary
{
    SoundSystem::~SoundSystem()
    {
        StopAll();
        StopBGM();
    }

    void SoundSystem::Load(const std::string& name, const std::wstring& filePath)
    {
        // 절대 경로로 변환해서 저장
        wchar_t fullPath[MAX_PATH];
        if (GetFullPathNameW(filePath.c_str(), MAX_PATH, fullPath, nullptr) > 0)
        {
            m_sounds[name] = fullPath;
        }
        else
        {
            m_sounds[name] = filePath;
        }
    }

    void SoundSystem::Play(const std::string& name)
    {
        if (m_sfxVolume <= 0.0f)
        {
            return;
        }

        auto it = m_sounds.find(name);
        if (it == m_sounds.end())
        {
            return;
        }

        // 이전 같은 이름의 사운드 정리
        auto aliasIt = m_activeAliases.find(name);
        if (aliasIt != m_activeAliases.end())
        {
            CloseAlias(aliasIt->second);
        }

        // 고유 alias 생성
        std::wstring alias = L"sfx" + std::to_wstring(m_aliasCounter++);

        // 열기
        std::wstring openCmd = L"open \"" + it->second + L"\" type mpegvideo alias " + alias;
        MCIERROR err = mciSendStringW(openCmd.c_str(), nullptr, 0, nullptr);
        if (err != 0)
        {
            return;
        }

        // 볼륨 설정
        auto mciVolume = static_cast<int32_t>(m_sfxVolume * 1000);
        std::wstring volCmd = std::format(L"setaudio {} volume to {}", alias, mciVolume);
        mciSendStringW(volCmd.c_str(), nullptr, 0, nullptr);

        // 재생
        std::wstring playCmd = L"play " + alias;
        mciSendStringW(playCmd.c_str(), nullptr, 0, nullptr);

        m_activeAliases[name] = alias;
    }

    void SoundSystem::StopAll()
    {
        for (auto& [name, alias] : m_activeAliases)
        {
            CloseAlias(alias);
        }
        m_activeAliases.clear();
    }

    void SoundSystem::CloseAlias(const std::wstring& alias)
    {
        std::wstring stopCmd = L"stop " + alias;
        std::wstring closeCmd = L"close " + alias;
        mciSendStringW(stopCmd.c_str(), nullptr, 0, nullptr);
        mciSendStringW(closeCmd.c_str(), nullptr, 0, nullptr);
    }

    void SoundSystem::PlayBGM(const std::wstring& filePath)
    {
        StopBGM();

        // 상대 경로를 절대 경로로 변환
        wchar_t fullPath[MAX_PATH];
        DWORD result = GetFullPathNameW(filePath.c_str(), MAX_PATH, fullPath, nullptr);
        if (result == 0)
        {
            Logger::Warning(L"Failed to get full path for BGM");
            return;
        }

        // 파일 존재 확인
        DWORD attr = GetFileAttributesW(fullPath);
        if (attr == INVALID_FILE_ATTRIBUTES)
        {
            Logger::Warning(L"BGM file not found: " + std::wstring(fullPath));
            return;
        }

        // mpegvideo는 WAV, MP3 모두 지원하고 repeat도 지원
        std::wstring openCmd = L"open \"" + std::wstring(fullPath) + L"\" type mpegvideo alias bgm";
        MCIERROR err = mciSendStringW(openCmd.c_str(), nullptr, 0, nullptr);
        if (err != 0)
        {
            wchar_t errMsg[256];
            mciGetErrorStringW(err, errMsg, 256);
            Logger::Warning(L"MCI open error: " + std::wstring(errMsg));
            return;
        }

        // 재생 전에 볼륨/음소거 설정
        if (m_bgmVolume <= 0.0f)
        {
            mciSendStringW(L"setaudio bgm off", nullptr, 0, nullptr);
        }
        else
        {
            auto mciVolume = static_cast<int32_t>(m_bgmVolume * 1000);
            std::wstring volCmd = std::format(L"setaudio bgm volume to {}", mciVolume);
            mciSendStringW(volCmd.c_str(), nullptr, 0, nullptr);
        }

        err = mciSendStringW(L"play bgm repeat", nullptr, 0, nullptr);
        if (err != 0)
        {
            wchar_t errMsg[256];
            mciGetErrorStringW(err, errMsg, 256);
            Logger::Warning(L"MCI play error: " + std::wstring(errMsg));
            mciSendStringW(L"close bgm", nullptr, 0, nullptr);
            return;
        }

        m_bgmPlaying = true;
        Logger::Info(L"BGM started: " + std::wstring(fullPath));
    }

    void SoundSystem::StopBGM()
    {
        if (m_bgmPlaying)
        {
            mciSendStringW(L"stop bgm", nullptr, 0, nullptr);
            mciSendStringW(L"close bgm", nullptr, 0, nullptr);
            m_bgmPlaying = false;
        }
    }

    void SoundSystem::SetBGMVolume(float volume)
    {
        m_bgmVolume = std::clamp(volume, 0.0f, 1.0f);

        if (m_bgmPlaying)
        {
            if (m_bgmVolume <= 0.0f)
            {
                // 음소거
                mciSendStringW(L"setaudio bgm off", nullptr, 0, nullptr);
            }
            else
            {
                // 음소거 해제 후 볼륨 설정
                mciSendStringW(L"setaudio bgm on", nullptr, 0, nullptr);
                auto mciVolume = static_cast<int32_t>(m_bgmVolume * 1000);
                std::wstring cmd = std::format(L"setaudio bgm volume to {}", mciVolume);
                mciSendStringW(cmd.c_str(), nullptr, 0, nullptr);
            }
        }
    }

    void SoundSystem::SetSFXVolume(float volume)
    {
        m_sfxVolume = std::clamp(volume, 0.0f, 1.0f);
    }
}