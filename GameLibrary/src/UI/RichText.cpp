//
// Created by Labae on 3/15/26.
//

#include "UI/RichText.hpp"

namespace GameLibrary
{
    ERichColor RichText::ToEnum(const std::string& name) noexcept
    {
        std::string lower = name;
        std::ranges::transform(lower, lower.begin(), ::tolower);
        if (lower == "white")
            return ERichColor::White;
        if (lower == "black")
            return ERichColor::Black;
        if (lower == "red")
            return ERichColor::Red;
        if (lower == "green")
            return ERichColor::Green;
        if (lower == "blue")
            return ERichColor::Blue;
        if (lower == "yellow")
            return ERichColor::Yellow;
        if (lower == "gold")
            return ERichColor::Gold;
        if (lower == "cyan")
            return ERichColor::Cyan;
        if (lower == "magenta")
            return ERichColor::Magenta;
        if (lower == "orange")
            return ERichColor::Orange;
        if (lower == "gray")
            return ERichColor::Gray;
        if (lower == "grey")
            return ERichColor::Gray;
        return ERichColor::White;
    }

    sf::Color RichText::ToColor(ERichColor color) noexcept
    {
        switch (color)
        {
        case ERichColor::White:
            return sf::Color::White;
        case ERichColor::Black:
            return sf::Color::Black;
        case ERichColor::Red:
            return sf::Color::Red;
        case ERichColor::Green:
            return sf::Color::Green;
        case ERichColor::Blue:
            return sf::Color::Blue;
        case ERichColor::Yellow:
            return sf::Color::Yellow;
        case ERichColor::Gold:
            return sf::Color{255, 216, 0, 255};
        case ERichColor::Cyan:
            return sf::Color::Cyan;
        case ERichColor::Magenta:
            return sf::Color::Magenta;
        case ERichColor::Orange:
            return sf::Color{255, 165, 0, 255};
        case ERichColor::Gray:
            return sf::Color{180, 180, 180, 255};
        default:
            return sf::Color::White;
        }
    }

    std::vector<RichTextSegment> RichText::Parse(const std::string& markup, const sf::Color& defaultColor)
    {
        std::vector<RichTextSegment> segments;
        sf::Color currentColor = defaultColor;
        std::string buffer;

        size_t i = 0;
        while (i < markup.size())
        {
            if (markup[i] == '[')
            {
                const auto closePos = markup.find(']', i + 1);
                if (closePos == std::string::npos)
                {
                    buffer += markup[i];
                    ++i;
                    continue;
                }

                const std::string tag = markup.substr(i + 1, closePos - i - 1);

                // 현재 버퍼 플러시
                if (not buffer.empty())
                {
                    segments.push_back({std::move(buffer), currentColor});
                    buffer.clear();
                }

                if (tag == "/")
                {
                    currentColor = defaultColor;
                }
                else
                {
                    const auto richColor = ToEnum(tag);
                    currentColor = (richColor == ERichColor::White) ? defaultColor : ToColor(richColor);
                }

                i = closePos + 1;
            }
            else
            {
                buffer += markup[i];
                ++i;
            }
        }

        if (not buffer.empty())
        {
            segments.push_back({std::move(buffer), currentColor});
        }

        return segments;
    }
} // namespace GameLibrary