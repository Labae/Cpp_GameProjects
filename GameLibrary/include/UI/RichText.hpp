//
// Created by Labae on 3/15/26.
//

#ifndef RICHTEXT_HPP
#define RICHTEXT_HPP

#include "SFML/Graphics/Color.hpp"

#include <vector>

namespace GameLibrary
{
    enum class ERichColor : uint8_t
    {
        White,
        Black,
        Red,
        Green,
        Blue,
        Yellow,
        Gold,
        Cyan,
        Magenta,
        Orange,
        Gray,
    };

    struct RichTextSegment
    {
        std::string text{};
        sf::Color color{sf::Color::White};
    };

    class RichText final
    {
    public:
        [[nodiscard]] static std::vector<RichTextSegment> Parse(const std::string& markup,
                                                                const sf::Color& defaultColor = sf::Color::White);

        [[nodiscard]] static ERichColor ToEnum(const std::string& name) noexcept;
        [[nodiscard]] static sf::Color ToColor(ERichColor color) noexcept;

    private:
        RichText() = default;
    };
} // namespace GameLibrary

#endif // RICHTEXT_HPP
