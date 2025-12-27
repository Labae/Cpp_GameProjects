#pragma once

#include <chrono>
#include "Interfaces/ITimeProvider.hpp"

namespace GameLibrary
{
    class TimeSystem : public ITimeProvider
    {
    public:
        TimeSystem() noexcept;

        void Update() noexcept;

        [[nodiscard]] float GetDeltaTime() const noexcept override;
        [[nodiscard]] float GetElapsedTime() const noexcept override;

    private:
        using Clock = std::chrono::steady_clock;
        using TimePoint = Clock::time_point;

        TimePoint m_startTime;
        TimePoint m_lastTime;
        float m_deltaTime = 0.0f;
    };
}