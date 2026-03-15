//
// Created by Labae on 3/14/26.
//

#include "Time/Timer.hpp"

namespace GameLibrary
{
    Timer::Timer(const float interval, std::function<void()> callback, const ETimerMode mode)
        : m_callback(std::move(callback))
        , m_interval(interval)
        , m_mode(mode)
    {
    }

    void Timer::Update(const float deltaTime)
    {
        if (m_finished)
        {
            return;
        }

        m_elapsed += deltaTime;

        while (m_elapsed >= m_interval)
        {
            m_callback();

            if (m_mode == ETimerMode::OneShot)
            {
                m_finished = true;
                return;
            }

            m_elapsed -= m_interval;
        }
    }

    void Timer::Reset() noexcept
    {
        m_elapsed = 0.0f;
        m_finished = false;
    }

    void Timer::SetInterval(const float interval) noexcept
    {
        m_interval = interval;
    }
} // namespace GameLibrary