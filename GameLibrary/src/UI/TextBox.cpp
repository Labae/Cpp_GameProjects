#include "UI/TextBox.hpp"

#include <sstream>

namespace GameLibrary
{
    void TextBox::SetText(const std::string& text)
    {
        m_lines.clear();
        std::istringstream stream(text);
        std::string line;

        while (std::getline(stream, line))
        {
            m_lines.emplace_back(std::move(line));
        }
    }

    void TextBox::SetPosition(const int32_t x, const int32_t y) noexcept
    {
        m_x = x;
        m_y = y;
    }

    void TextBox::SetSize(const int32_t width, const int32_t height) noexcept
    {
        m_width = width;
        m_height = height;
    }

    void TextBox::SetStyle(const TextBoxStyle& style) noexcept
    {
        m_style = style;
    }

    void TextBox::Render(IGraphics& graphics) const
    {
        // 배경
        graphics.FillRect(m_x, m_y, m_width, m_height, m_style.backgroundColor);
        graphics.DrawRect(m_x, m_y, m_width, m_height, m_style.borderColor);

        // 텍스트 시작 위치
        int32_t textX = m_x + m_style.paddingX;
        if (m_style.align == TextAlign::Center)
        {
            textX = m_x + m_width / 2;
        }
        else if (m_style.align == TextAlign::Right)
        {
            textX = m_x + m_width - m_style.paddingX;
        }

        const int32_t textY = m_y + m_style.paddingY;

        for (int32_t i = 0; i < static_cast<int32_t>(m_lines.size()); ++i)
        {
            const int32_t lineY = textY + i * m_style.lineSpacing;
            graphics.DrawRichText(m_lines[i], textX, lineY, m_style.fontSize, m_style.textColor, m_style.align);
        }
    }
} // namespace GameLibrary