#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include "Interfaces/IGraphics.hpp"

#include <SFML/Graphics/Color.hpp>
#include <cstdint>
#include <string>
#include <vector>

namespace GameLibrary
{
    struct TextBoxStyle
    {
        int32_t fontSize{20};
        int32_t lineSpacing{28};
        int32_t paddingX{16};
        int32_t paddingY{12};
        sf::Color textColor{255, 255, 255, 255};
        sf::Color backgroundColor{20, 20, 20, 220};
        sf::Color borderColor{150, 150, 150, 255};
        TextAlign align{TextAlign::Left};
    };

    class TextBox
    {
    public:
        TextBox() = default;
        ~TextBox() = default;

        void SetText(const std::string& text);
        void SetPosition(int32_t x, int32_t y) noexcept;
        void SetSize(int32_t width, int32_t height) noexcept;
        void SetStyle(const TextBoxStyle& style) noexcept;

        void Render(IGraphics& graphics) const;

        [[nodiscard]] int32_t GetLineCount() const noexcept { return static_cast<int32_t>(m_lines.size()); }

    private:
        std::vector<std::string> m_lines{};
        TextBoxStyle m_style{};
        int32_t m_x{};
        int32_t m_y{};
        int32_t m_width{};
        int32_t m_height{};
    };
} // namespace GameLibrary

#endif // TEXTBOX_HPP