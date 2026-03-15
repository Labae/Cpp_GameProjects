#ifndef MENUSELECTOR_HPP
#define MENUSELECTOR_HPP

#include "Interfaces/IGraphics.hpp"
#include "Interfaces/IInputProvider.hpp"

#include <SFML/Graphics/Color.hpp>
#include <cstdint>
#include <string>
#include <vector>

namespace GameLibrary
{
    struct MenuStyle
    {
        int32_t fontSize{24};
        int32_t selectedFontSize{28};
        int32_t spacing{40};
        sf::Color normalColor{180, 180, 180, 255};
        sf::Color selectedColor{255, 255, 0, 255};
        TextAlign align{TextAlign::Left};
    };

    class MenuSelector
    {
    public:
        explicit MenuSelector(IInputProvider& input) noexcept;
        ~MenuSelector() = default;

        MenuSelector(const MenuSelector&) = delete;
        MenuSelector(MenuSelector&&) = default;
        MenuSelector& operator=(const MenuSelector&) = delete;
        MenuSelector& operator=(MenuSelector&&) = default;

        void SetItems(const std::vector<std::string>& items);
        void SetPosition(int32_t x, int32_t y) noexcept;
        void SetStyle(const MenuStyle& style) noexcept;

        void Update();
        void Render(IGraphics& graphics) const;

        [[nodiscard]] bool IsConfirmed() const noexcept { return m_confirmed; }
        [[nodiscard]] bool IsCancelled() const noexcept { return m_cancelled; }
        [[nodiscard]] int32_t GetSelectedIndex() const noexcept { return m_selectedIndex; }
        [[nodiscard]] const std::string& GetSelectedItem() const;

        void Reset() noexcept;

    private:
        void MoveUp() noexcept;
        void MoveDown() noexcept;

        IInputProvider& m_input;

        std::vector<std::string> m_items{};
        MenuStyle m_style{};
        int32_t m_x{};
        int32_t m_y{};
        int32_t m_selectedIndex{};
        bool m_confirmed{};
        bool m_cancelled{};
    };
} // namespace GameLibrary

#endif // MENUSELECTOR_HPP