#pragma once

#include "Interfaces/ITimeProvider.hpp"

#include <chrono>

namespace GameLibrary
{
    class TimeSystem : public ITimeProvider
    {
    public:
        TimeSystem() noexcept;
        ~TimeSystem() = default;

        TimeSystem(const TimeSystem&) = delete;
        TimeSystem(TimeSystem&&) = delete;
        TimeSystem& operator=(const TimeSystem&) = delete;
        TimeSystem& operator=(TimeSystem&&) = delete;

        void Update() noexcept override;

        [[nodiscard]] float GetDeltaTime() const noexcept override;
        [[nodiscard]] float GetElapsedTime() const noexcept override;

    private:
        using Clock = std::chrono::steady_clock;
        using TimePoint = Clock::time_point;

        TimePoint m_startTime{};
        TimePoint m_lastTime{};
        float m_deltaTime{};
    };
} // namespace GameLibrary