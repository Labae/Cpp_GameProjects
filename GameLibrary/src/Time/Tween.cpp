#include "Time/Tween.hpp"

#include <algorithm>

namespace GameLibrary
{
    Tween::Tween(const float from, const float to, const float duration, const EaseType easeType,
                 std::function<void(float)> onUpdate, std::function<void()> onComplete)
        : m_easeFunction(Easing::FromType(easeType))
        , m_onUpdate(std::move(onUpdate))
        , m_onComplete(std::move(onComplete))
        , m_from(from)
        , m_to(to)
        , m_duration(duration)
        , m_currentValue(from)
    {
    }

    Tween::Tween(const float from, const float to, const float duration, const EaseFunction easeFunction,
                 std::function<void(float)> onUpdate, std::function<void()> onComplete)
        : m_easeFunction(easeFunction)
        , m_onUpdate(std::move(onUpdate))
        , m_onComplete(std::move(onComplete))
        , m_from(from)
        , m_to(to)
        , m_duration(duration)
        , m_currentValue(from)
    {
    }

    void Tween::Update(const float deltaTime)
    {
        if (m_finished)
        {
            return;
        }

        m_elapsed = std::min(m_elapsed + deltaTime, m_duration);
        const float t = m_easeFunction(m_elapsed / m_duration);
        m_currentValue = m_from + (m_to - m_from) * t;

        m_onUpdate(m_currentValue);

        if (m_elapsed >= m_duration)
        {
            m_finished = true;

            if (m_onComplete)
            {
                m_onComplete();
            }
        }
    }

    void Tween::Reset() noexcept
    {
        m_elapsed = 0.0f;
        m_currentValue = m_from;
        m_finished = false;
    }
} // namespace GameLibrary