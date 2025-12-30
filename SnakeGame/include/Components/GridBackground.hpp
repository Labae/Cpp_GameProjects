#pragma once

#include "GameObject/Component.hpp"
#include "Interfaces/IGraphics.hpp"

#include <SFML/Graphics/Color.hpp>

class GridBackground : public GameLibrary::Component
{
public:
    GridBackground(int32_t screenWidth, int32_t screenHeight, int32_t gridSize,
                   const sf::Color& lineColor = sf::Color(40, 40, 40, 255));

    void Render(GameLibrary::IGraphics& graphics) override;

    void SetLineColor(const sf::Color& color) noexcept { m_lineColor = color; }

private:
    int32_t m_screenWidth{};
    int32_t m_screenHeight{};
    int32_t m_gridSize{};
    sf::Color m_lineColor{};
};