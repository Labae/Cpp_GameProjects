#ifndef EASING_HPP
#define EASING_HPP

#include <cmath>

namespace GameLibrary
{
    using EaseFunction = float(*)(float);

    enum class EaseType
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

        [[nodiscard]] inline EaseFunction FromType(const EaseType type) noexcept
        {
            switch (type)
            {
            case EaseType::Linear:      return Linear;
            case EaseType::InQuad:      return InQuad;
            case EaseType::OutQuad:     return OutQuad;
            case EaseType::InOutQuad:   return InOutQuad;
            case EaseType::InCubic:     return InCubic;
            case EaseType::OutCubic:    return OutCubic;
            case EaseType::InOutCubic:  return InOutCubic;
            case EaseType::InExpo:      return InExpo;
            case EaseType::OutExpo:     return OutExpo;
            case EaseType::InOutExpo:   return InOutExpo;
            case EaseType::InBack:      return InBack;
            case EaseType::OutBack:     return OutBack;
            case EaseType::InOutBack:   return InOutBack;
            }
            return Linear;
        }
    } // namespace Easing
} // namespace GameLibrary

#endif // EASING_HPP