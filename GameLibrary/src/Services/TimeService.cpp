#include "Services/TimeService.hpp"

namespace GameLibrary
{
    TimeService::TimeService() noexcept : m_startTime(Clock::now()), m_lastTime(m_startTime) {}

    void TimeService::Update() noexcept
    {
        const auto now = Clock::now();
        m_deltaTime = std::chrono::duration<float>(now - m_lastTime).count();
        m_lastTime = now;
    }

    float TimeService::GetDeltaTime() const noexcept
    {
        return m_deltaTime;
    }

    float TimeService::GetElapsedTime() const noexcept
    {
        const auto now = Clock::now();
        return std::chrono::duration<float>(now - m_startTime).count();
    }
} // namespace GameLibrary