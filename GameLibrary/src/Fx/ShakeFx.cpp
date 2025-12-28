#include "Fx/ShakeFx.hpp"

#include <chrono>
#include <random>

namespace GameLibrary
{
    ShakeFx::ShakeFx(float intensity, float duration) : m_intensity(intensity), m_duration(duration) {}

    void ShakeFx::Update(float deltaTime)
    {
        m_elapsed += deltaTime;

        if (m_elapsed >= m_duration)
        {
            m_finished = true;
            m_offsetX = 0;
            m_offsetY = 0;
            return;
        }

        const float ratio = 1.0f - (m_elapsed / m_duration);
        const float currentIntensity = m_intensity * ratio;

        static std::mt19937 gen{std::random_device{}()};
        std::uniform_real_distribution<float> dist(-currentIntensity, currentIntensity);

        m_offsetX = static_cast<int32_t>(dist(gen));
        m_offsetY = static_cast<int32_t>(dist(gen));
    }

    void ShakeFx::Render([[maybe_unused]] IGraphics& graphics) {}
} // namespace GameLibrary