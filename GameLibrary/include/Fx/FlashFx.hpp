#pragma once

#include "Fx.hpp"
#include "Interfaces/IGraphics.hpp"

#include <SFML/Graphics/Color.hpp>

namespace GameLibrary
{
    class FlashFx : public Fx
    {
    public:
        FlashFx(int32_t screenWidth, int32_t screenHeight, const sf::Color& color, float duration, int32_t count = 1);

        void Update(float deltaTime) override;
        void Render(IGraphics& graphics) override;

    private:
        int32_t m_screenWidth{};
        int32_t m_screenHeight{};
        sf::Color m_color{};
        float m_duration{};
        int32_t m_count{};
        float m_elapsed{};
    };
} // namespace GameLibrary