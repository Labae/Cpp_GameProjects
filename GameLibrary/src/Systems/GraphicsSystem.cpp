#include "Systems/GraphicsSystem.hpp"

#include "Core/ServiceContainer.hpp"
#include "Platform/Window.hpp"
#include "Systems/ConfigSystem.hpp"
#include "Systems/Logger.hpp"

namespace GameLibrary
{
    void GraphicsSystem::Init(ServiceContainer& container)
    {
        m_window = &container.Resolve<Window>()->GetHandle();
        auto* config = container.Resolve<ConfigSystem>();
        const std::string fontPath = config->GetString("fontPath", "");
        if (fontPath.empty())
        {
            Logger::Warning("fontPath is empty");
        }
        else
        {
            if (m_font.openFromFile(fontPath))
            {
                m_fontLoaded = true;
            }
            else
            {
                Logger::Warning("Failed to open font from file: " + fontPath);
            }
        }
    }

    void GraphicsSystem::Clear(const sf::Color& color) noexcept
    {
        if (m_window)
        {
            m_window->clear(color);
        }
    }

    void GraphicsSystem::DrawRect(int32_t x, int32_t y, int32_t width, int32_t height, const sf::Color& color) noexcept
    {
        if (!m_window)
        {
            return;
        }

        sf::RectangleShape rect(sf::Vector2f(width, height));
        rect.setPosition(sf::Vector2f(x + m_offsetX, y + m_offsetY));
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(color);
        rect.setOutlineThickness(1.0f);

        m_window->draw(rect);
    }

    void GraphicsSystem::FillRect(int32_t x, int32_t y, int32_t width, int32_t height, const sf::Color& color) noexcept
    {
        if (!m_window)
        {
            return;
        }

        sf::RectangleShape rect(sf::Vector2f(width, height));
        rect.setPosition(sf::Vector2f(x + m_offsetX, y + m_offsetY));
        rect.setFillColor(color);

        m_window->draw(rect);
    }

    void GraphicsSystem::DrawLabel(const std::string& text, int32_t x, int32_t y, int32_t fontSize,
                                   const sf::Color& color, TextAlign align) noexcept
    {
        if (!m_window)
        {
            return;
        }

        if (!m_fontLoaded)
        {
            return;
        }

        sf::Text sfText(m_font, text, fontSize);
        sfText.setFillColor(color);

        const sf::FloatRect bounds = sfText.getLocalBounds();
        float posX = static_cast<float>(x + m_offsetX);

        switch (align)
        {
        case TextAlign::Center:
            posX -= bounds.size.x * 0.5f;
            break;
        case TextAlign::Right:
            posX -= bounds.size.x;
            break;
        case TextAlign::Left:
        default:
            break;
        }

        sfText.setPosition(sf::Vector2f(posX, y + m_offsetY));
        m_window->draw(sfText);
    }

    void GraphicsSystem::Present() noexcept
    {
        if (m_window)
        {
            m_window->display();
        }
    }
} // namespace GameLibrary