#include "Fx/FlashFx.hpp"

#include <cmath>

namespace GameLibrary
{
    FlashFx::FlashFx(const int32_t screenWidth, const int32_t screenHeight, const sf::Color& color,
                     const float duration, const int32_t count)
        : m_screenWidth(screenWidth)
        , m_screenHeight(screenHeight)
        , m_color(color)
        , m_count(count)
        , m_tween(0.0f, 1.0f, duration, EEase::Linear,
                  [this](const float progress)
                  {
                      const float cycleTime = 1.0f / static_cast<float>(m_count);
                      const float cycleProgress = std::fmod(progress, cycleTime) / cycleTime;
                      m_alpha = cycleProgress < 0.5f
                                    ? cycleProgress * 2.0f
                                    : (1.0f - cycleProgress) * 2.0f;
                  },
                  [this]() { m_finished = true; })
    {
    }

    void FlashFx::Update(const float deltaTime)
    {
        m_tween.Update(deltaTime);
    }

    void FlashFx::Render(IGraphics& graphics)
    {
        if (m_finished)
        {
            return;
        }

        const auto finalAlpha = static_cast<uint8_t>(static_cast<float>(m_color.a) * m_alpha);
        const sf::Color color = {m_color.r, m_color.g, m_color.b, finalAlpha};
        graphics.FillRect(0, 0, m_screenWidth, m_screenHeight, color);
    }
} // namespace GameLibrary