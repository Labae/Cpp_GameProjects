#pragma once

#include <SFML/Graphics/Color.hpp>
#include <string>
#include "UI/RichText.hpp"

namespace GameLibrary
{
    enum class TextAlign
    {
        Left,
        Center,
        Right
    };

    class IGraphics
    {
    public:
        virtual ~IGraphics() = default;

        virtual void Clear(const sf::Color& color) = 0;
        virtual void DrawRect(int32_t x, int32_t y, int32_t width, int32_t height, const sf::Color& color) = 0;
        virtual void DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const sf::Color& color) = 0;
        virtual void FillRect(int32_t x, int32_t y, int32_t width, int32_t height, const sf::Color& color) = 0;
        virtual void DrawLabel(const std::string& text, int32_t x, int32_t y, int32_t fontSize, const sf::Color& color,
                               TextAlign align = TextAlign::Left) = 0;
        virtual void DrawRichText(const std::string& markup, int32_t x, int32_t y, int32_t fontSize,
                                          const sf::Color& defaultColor = sf::Color::White,
                                          TextAlign align = TextAlign::Left) = 0;
        virtual void Present() = 0;

        virtual void SetOffset(int32_t x, int32_t y) = 0;
        virtual void ResetOffset() = 0;
    };
} // namespace GameLibrary