#ifndef TIMER_HPP
#define TIMER_HPP

#include <functional>

namespace GameLibrary
{
    enum class TimerMode
    {
        Repeat,
        OneShot
    };

    class Timer
    {
    public:
        Timer(float interval, std::function<void()> callback, TimerMode mode = TimerMode::Repeat);
        ~Timer() = default;

        Timer(const Timer&) = delete;
        Timer(Timer&&) = default;
        Timer& operator=(const Timer&) = delete;
        Timer& operator=(Timer&&) = default;

        void Update(float deltaTime);
        void Reset() noexcept;

        void SetInterval(float interval) noexcept;
        [[nodiscard]] float GetInterval() const noexcept { return m_interval; }
        [[nodiscard]] float GetElapsed() const noexcept { return m_elapsed; }
        [[nodiscard]] bool IsFinished() const noexcept { return m_finished; }

    private:
        std::function<void()> m_callback;
        float m_interval{};
        float m_elapsed{};
        TimerMode m_mode{};
        bool m_finished{};
    };
} // namespace GameLibrary

#endif // TIMER_HPP