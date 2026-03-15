#include "Fx/ShakeFx.hpp"

#include "Utility/Random.hpp"

namespace GameLibrary
{
    ShakeFx::ShakeFx(const float intensity, const float duration)
        : m_tween(intensity, 0.0f, duration, EEase::Linear,
                  [this](const float value)
                  {
                      m_offsetX = static_cast<int32_t>(Random::Range(-value, value));
                      m_offsetY = static_cast<int32_t>(Random::Range(-value, value));
                  },
                  [this]()
                  {
                      m_finished = true;
                      m_offsetX = 0;
                      m_offsetY = 0;
                  })
    {
    }

    void ShakeFx::Update(const float deltaTime)
    {
        m_tween.Update(deltaTime);
    }

    void ShakeFx::Render([[maybe_unused]] IGraphics& graphics) {}
} // namespace GameLibrary