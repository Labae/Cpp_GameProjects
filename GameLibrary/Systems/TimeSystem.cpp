#include "TimeSystem.hpp"

namespace GameLibrary
{
    TimeSystem::TimeSystem() noexcept
        : m_startTime(Clock::now())
        , m_lastTime(m_startTime)
    {
    }

    void TimeSystem::Update() noexcept
    {
        auto now = Clock::now();
        m_deltaTime = std::chrono::duration<float>(now - m_lastTime).count();
        m_lastTime = now;
    }

    float TimeSystem::GetDeltaTime() const noexcept
    {
        return m_deltaTime;
    }

    float TimeSystem::GetElapsedTime() const noexcept
    {
        auto now = Clock::now();
        return std::chrono::duration<float>(now - m_startTime).count();
    }
}