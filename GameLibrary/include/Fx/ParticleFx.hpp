#pragma once

#include "Fx.hpp"
#include "Interfaces/IGraphics.hpp"

#include <SFML/Graphics/Color.hpp>
#include <vector>

namespace GameLibrary
{
    struct ParticleFxConfig
    {
        sf::Color color{sf::Color::White};
        size_t count{8};
        float duration{0.5f};
        float size{8.0f};
        float speed{100.0f};

        uint32_t startAlpha{255};
        uint32_t endAlpha{0};
    };

    class ParticleFx : public Fx
    {
    public:
        ParticleFx(float x, float y, const ParticleFxConfig& config = {});

        void Update(float deltaTime) override;
        void Render(IGraphics& graphics) override;

    private:
        struct Particle
        {
            float x{};
            float y{};
            float vx{};
            float vy{};
            float life{};
        };

        std::vector<Particle> m_particles{};
        ParticleFxConfig m_config{};
        float m_elapsed{};
    };
} // namespace GameLibrary