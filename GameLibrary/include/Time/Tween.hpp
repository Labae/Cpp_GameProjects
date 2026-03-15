#ifndef TWEEN_HPP
#define TWEEN_HPP

#include "Easing.hpp"

#include <functional>

namespace GameLibrary
{
    class Tween
    {
    public:
        Tween(float from, float to, float duration, EEase easeType,
              std::function<void(float)> onUpdate,
              std::function<void()> onComplete = nullptr);

        Tween(float from, float to, float duration, EaseFunction easeFunction,
              std::function<void(float)> onUpdate,
              std::function<void()> onComplete = nullptr);

        ~Tween() = default;

        Tween(const Tween&) = delete;
        Tween(Tween&&) = default;
        Tween& operator=(const Tween&) = delete;
        Tween& operator=(Tween&&) = default;

        void Update(float deltaTime);
        void Reset() noexcept;

        [[nodiscard]] bool IsFinished() const noexcept { return m_finished; }
        [[nodiscard]] float GetElapsed() const noexcept { return m_elapsed; }
        [[nodiscard]] float GetCurrentValue() const noexcept { return m_currentValue; }

    private:
        EaseFunction m_easeFunction{};
        std::function<void(float)> m_onUpdate;
        std::function<void()> m_onComplete;

        float m_from{};
        float m_to{};
        float m_duration{};
        float m_elapsed{};
        float m_currentValue{};
        bool m_finished{};
    };
} // namespace GameLibrary

#endif // TWEEN_HPP