#include "GridBackground.hpp"

GridBackground::GridBackground(int screenWidth, int screenHeight, int gridSize, const GameLibrary::Color& lineColor)
    : m_screenWidth(screenWidth)
    , m_screenHeight(screenHeight)
    , m_gridSize(gridSize)
    , m_lineColor(lineColor)
{
}

void GridBackground::Render(GameLibrary::IGraphics& graphics)
{
    // 수직선
    for (int x = 0; x <= m_screenWidth; x += m_gridSize)
    {
        graphics.DrawRect(x, 0, 1, m_screenHeight, m_lineColor);
    }

    // 수평선
    for (int y = 0; y <= m_screenHeight; y += m_gridSize)
    {
        graphics.DrawRect(0, y, m_screenWidth, 1, m_lineColor);
    }
}