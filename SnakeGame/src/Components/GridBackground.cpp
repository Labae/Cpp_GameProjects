#include "Components/GridBackground.hpp"

GridBackground::GridBackground(GameLibrary::Actor* owner, const int32_t screenWidth, const int32_t screenHeight,
                               const int32_t gridSize,
                               const sf::Color& lineColor)
    : Component(owner)
    , m_screenWidth(screenWidth)
    , m_screenHeight(screenHeight)
    , m_gridSize(gridSize)
    , m_lineColor(lineColor)
{
}

void GridBackground::Render(GameLibrary::IGraphics& graphics)
{
    // 수직선
    for (int32_t x = 0; x <= m_screenWidth; x += m_gridSize)
    {
        graphics.DrawRect(x, 0, 1, m_screenHeight, m_lineColor);
    }

    // 수평선
    for (int32_t y = 0; y <= m_screenHeight; y += m_gridSize)
    {
        graphics.DrawRect(0, y, m_screenWidth, 1, m_lineColor);
    }
}