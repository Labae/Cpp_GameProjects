#include "ParticleFx.hpp"
#include <cmath>
#include <random>
#include <chrono>

namespace GameLibrary
{
    ParticleFx::ParticleFx(float x, float y, const ParticleFxConfig& config)
        : m_config(config)
    {
        // 매번 다른 시드 사용
        auto seed = static_cast<unsigned>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()
            );
        std::mt19937 gen(seed);
        std::uniform_real_distribution<float> angleDist(0.0f, 6.28318f);
        std::uniform_real_distribution<float> speedDist(config.speed * 0.5f, config.speed * 1.5f);

        m_particles.reserve(config.count);

        for (size_t i = 0; i < config.count; ++i)
        {
            float angle = angleDist(gen);
            float speed = speedDist(gen);

            Particle p;
            p.x = x;
            p.y = y;
            p.vx = std::cos(angle) * speed;
            p.vy = std::sin(angle) * speed;
            p.life = 1.0f;

            m_particles.push_back(p);
        }
    }

    void ParticleFx::Update(float deltaTime)
    {
        m_elapsed += deltaTime;

        if (m_elapsed >= m_config.duration)
        {
            m_finished = true;
            return;
        }

        float lifeRatio = 1.0f - (m_elapsed / m_config.duration);

        for (auto& p : m_particles)
        {
            p.x += p.vx * deltaTime;
            p.y += p.vy * deltaTime;
            p.life = lifeRatio;

            // 감속
            p.vx *= 0.95f;
            p.vy *= 0.95f;
        }
    }

    void ParticleFx::Render(IGraphics& graphics)
    {
        for (const auto& p : m_particles)
        {
            auto alpha = static_cast<uint8_t>(
                m_config.startAlpha * p.life + m_config.endAlpha * (1.0f - p.life)
                );
            Color color = { m_config.color.r, m_config.color.g, m_config.color.b, alpha };

            auto size = static_cast<int32_t>(m_config.size * p.life) + 2;
            graphics.FillRect(
                static_cast<int32_t>(p.x) - size / 2,
                static_cast<int32_t>(p.y) - size / 2,
                size,
                size,
                color
            );
        }
    }
}