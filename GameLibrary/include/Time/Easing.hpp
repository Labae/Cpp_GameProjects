#ifndef EASING_HPP
#define EASING_HPP

#include <cmath>

namespace GameLibrary
{
    using EaseFunction = float(*)(float);

    enum class EEase
    {
        Linear,
        InQuad,
        OutQuad,
        InOutQuad,
        InCubic,
        OutCubic,
        InOutCubic,
        InExpo,
        OutExpo,
        InOutExpo,
        InBack,
        OutBack,
        InOutBack
    };

    namespace Easing
    {
        inline float Linear(const float t) noexcept { return t; }

        inline float InQuad(const float t) noexcept { return t * t; }
        inline float OutQuad(const float t) noexcept { return t * (2.0f - t); }
        inline float InOutQuad(const float t) noexcept
        {
            return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
        }

        inline float InCubic(const float t) noexcept { return t * t * t; }
        inline float OutCubic(const float t) noexcept
        {
            const float u = t - 1.0f;
            return u * u * u + 1.0f;
        }
        inline float InOutCubic(const float t) noexcept
        {
            return t < 0.5f ? 4.0f * t * t * t : 1.0f + (t - 1.0f) * (2.0f * t - 2.0f) * (2.0f * t - 2.0f);
        }

        inline float InExpo(const float t) noexcept
        {
            return t == 0.0f ? 0.0f : std::pow(2.0f, 10.0f * (t - 1.0f));
        }
        inline float OutExpo(const float t) noexcept
        {
            return t == 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
        }
        inline float InOutExpo(const float t) noexcept
        {
            if (t == 0.0f) return 0.0f;
            if (t == 1.0f) return 1.0f;
            return t < 0.5f
                ? std::pow(2.0f, 20.0f * t - 10.0f) * 0.5f
                : (2.0f - std::pow(2.0f, -20.0f * t + 10.0f)) * 0.5f;
        }

        inline float InBack(const float t) noexcept
        {
            constexpr float s = 1.70158f;
            return t * t * ((s + 1.0f) * t - s);
        }
        inline float OutBack(const float t) noexcept
        {
            constexpr float s = 1.70158f;
            const float u = t - 1.0f;
            return u * u * ((s + 1.0f) * u + s) + 1.0f;
        }
        inline float InOutBack(const float t) noexcept
        {
            constexpr float s = 1.70158f * 1.525f;
            if (t < 0.5f)
            {
                return (4.0f * t * t * ((s + 1.0f) * 2.0f * t - s)) * 0.5f;
            }
            const float u = 2.0f * t - 2.0f;
            return (u * u * ((s + 1.0f) * u + s) + 2.0f) * 0.5f;
        }

        [[nodiscard]] inline EaseFunction FromType(const EEase type) noexcept
        {
            switch (type)
            {
            case EEase::Linear:      return Linear;
            case EEase::InQuad:      return InQuad;
            case EEase::OutQuad:     return OutQuad;
            case EEase::InOutQuad:   return InOutQuad;
            case EEase::InCubic:     return InCubic;
            case EEase::OutCubic:    return OutCubic;
            case EEase::InOutCubic:  return InOutCubic;
            case EEase::InExpo:      return InExpo;
            case EEase::OutExpo:     return OutExpo;
            case EEase::InOutExpo:   return InOutExpo;
            case EEase::InBack:      return InBack;
            case EEase::OutBack:     return OutBack;
            case EEase::InOutBack:   return InOutBack;
            }
            return Linear;
        }
    } // namespace Easing
} // namespace GameLibrary

#endif // EASING_HPP