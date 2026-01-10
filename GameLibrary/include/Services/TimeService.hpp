#pragma once

#include <chrono>

namespace GameLibrary
{
    class TimeService
    {
    public:
        TimeService() noexcept;
        ~TimeService() = default;

        TimeService(const TimeService&) = delete;
        TimeService(TimeService&&) = delete;
        TimeService& operator=(const TimeService&) = delete;
        TimeService& operator=(TimeService&&) = delete;

        void Update() noexcept;

        [[nodiscard]] float GetDeltaTime() const noexcept;
        [[nodiscard]] float GetElapsedTime() const noexcept;

    private:
        using Clock = std::chrono::steady_clock;
        using TimePoint = Clock::time_point;

        TimePoint m_startTime{};
        TimePoint m_lastTime{};
        float m_deltaTime{};
    };
} // namespace GameLibrary