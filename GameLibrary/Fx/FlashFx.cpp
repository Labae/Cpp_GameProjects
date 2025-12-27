#include "FlashFx.hpp"
#include <cmath>

namespace GameLibrary
{
    FlashFx::FlashFx(int32_t screenWidth, int32_t screenHeight, const Color& color, float duration, int count)
        : m_screenWidth(screenWidth)
        , m_screenHeight(screenHeight)
        , m_color(color)
        , m_duration(duration)
        , m_count(count)
    {
    }

    void FlashFx::Update(float deltaTime)
    {
        m_elapsed += deltaTime;

        if (m_elapsed >= m_duration)
        {
            m_finished = true;
        }
    }

    void FlashFx::Render(IGraphics& graphics)
    {
        if (m_finished)
        {
            return;
        }

        float cycleTime = m_duration / m_count;
        float cycleProgress = std::fmod(m_elapsed, cycleTime) / cycleTime;

        float alpha;
        if (cycleProgress < 0.5f)
        {
            alpha = cycleProgress * 2.0f;   // 0->1
        }
        else
        {
            alpha = (1.0f - cycleProgress) * 2.0f; // 1->0
        }

        auto finalAlpha = static_cast<uint8_t>(m_color.a * alpha);
        Color color = { m_color.r, m_color.g, m_color.b, finalAlpha };
        graphics.FillRect(0, 0, m_screenWidth, m_screenHeight, color);
    }
}