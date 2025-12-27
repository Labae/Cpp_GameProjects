#pragma once

#include "GameObject/Component.hpp"
#include "Interfaces/IGraphics.hpp"

class GridBackground : public GameLibrary::Component
{
public:
    GridBackground(int screenWidth, int screenHeight, int gridSize, const GameLibrary::Color& lineColor = { 40, 40, 40, 255 });

    void Render(GameLibrary::IGraphics& graphics) override;

    void SetLineColor(const GameLibrary::Color& color) { m_lineColor = color; }

private:
    int m_screenWidth;
    int m_screenHeight;
    int m_gridSize;
    GameLibrary::Color m_lineColor;
};