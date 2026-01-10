#include "Services/GraphicsService.hpp"

#include "Services/ConfigService.hpp"
#include "Systems/Logger.hpp"
#include "Core/ServiceContainer.hpp"
#include "Platform/Window.hpp"
#include "Services/ResourceService.hpp"

namespace GameLibrary
{
    void GraphicsService::Init(ServiceContainer& container)
    {
        m_window = &container.Resolve<Window>()->GetHandle();
        m_resourceService = container.Resolve<ResourceService>();
    }

    void GraphicsService::Clear(const sf::Color& color) noexcept
    {
        if (m_window)
        {
            m_window->clear(color);
        }
    }

    void GraphicsService::DrawRect(const int32_t x, const int32_t y, const int32_t width, const int32_t height, const sf::Color& color) noexcept
    {
        if (not m_window)
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

    void GraphicsService::FillRect(const int32_t x, const int32_t y, const int32_t width, const int32_t height, const sf::Color& color) noexcept
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

    void GraphicsService::DrawLabel(const std::string& text, int32_t x, int32_t y, int32_t fontSize,
                                   const sf::Color& color, TextAlign align) noexcept
    {
        if (not m_window or not m_resourceService)
        {
            return;
        }

        const auto font = m_resourceService->GetFont("main");

        sf::Text sfText(font, text, fontSize);
        sfText.setFillColor(color);

        const sf::FloatRect bounds = sfText.getLocalBounds();
        auto posX = static_cast<float>(x + m_offsetX);

        switch (align)
        {
        case TextAlign::Center:
        {
            posX -= bounds.size.x * 0.5f;
            break;
        }
        case TextAlign::Right:
        {
            posX -= bounds.size.x;
            break;
        }
        case TextAlign::Left:
        default:
            break;
        }

        sfText.setPosition(sf::Vector2f(posX, y + m_offsetY));
        m_window->draw(sfText);
    }

    void GraphicsService::Present() noexcept
    {
        if (m_window)
        {
            m_window->display();
        }
    }
} // namespace GameLibrary