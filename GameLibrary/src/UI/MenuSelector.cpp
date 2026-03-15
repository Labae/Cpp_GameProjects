#include "UI/MenuSelector.hpp"

namespace GameLibrary
{
    MenuSelector::MenuSelector(IInputProvider& input) noexcept : m_input(input) {}

    void MenuSelector::SetItems(const std::vector<std::string>& items)
    {
        m_items = items;
        m_selectedIndex = 0;
        m_confirmed = false;
        m_cancelled = false;
    }

    void MenuSelector::SetPosition(const int32_t x, const int32_t y) noexcept
    {
        m_x = x;
        m_y = y;
    }

    void MenuSelector::SetStyle(const MenuStyle& style) noexcept
    {
        m_style = style;
    }

    void MenuSelector::Update()
    {
        m_confirmed = false;
        m_cancelled = false;

        if (m_items.empty())
        {
            return;
        }

        if (m_input.IsKeyPressed(KeyCode::Up))
        {
            MoveUp();
        }
        if (m_input.IsKeyPressed(KeyCode::Down))
        {
            MoveDown();
        }
        if (m_input.IsKeyPressed(KeyCode::Enter) or m_input.IsKeyPressed(KeyCode::Space))
        {
            m_confirmed = true;
        }
        if (m_input.IsKeyPressed(KeyCode::Escape))
        {
            m_cancelled = true;
        }
    }

    void MenuSelector::Render(IGraphics& graphics) const
    {
        for (int32_t i = 0; i < static_cast<int32_t>(m_items.size()); ++i)
        {
            const bool isSelected = (i == m_selectedIndex);
            const int32_t fontSize = isSelected ? m_style.selectedFontSize : m_style.fontSize;
            const sf::Color& color = isSelected ? m_style.selectedColor : m_style.normalColor;
            const int32_t y = m_y + i * m_style.spacing;

            graphics.DrawLabel(m_items[i], m_x, y, fontSize, color, m_style.align);
        }
    }

    const std::string& MenuSelector::GetSelectedItem() const
    {
        return m_items[m_selectedIndex];
    }

    void MenuSelector::Reset() noexcept
    {
        m_selectedIndex = 0;
        m_confirmed = false;
        m_cancelled = false;
    }

    void MenuSelector::MoveUp() noexcept
    {
        const auto count = static_cast<int32_t>(m_items.size());
        m_selectedIndex = (m_selectedIndex - 1 + count) % count;
    }

    void MenuSelector::MoveDown() noexcept
    {
        const auto count = static_cast<int32_t>(m_items.size());
        m_selectedIndex = (m_selectedIndex + 1) % count;
    }
} // namespace GameLibrary