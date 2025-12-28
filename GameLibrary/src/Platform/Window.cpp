#include "Platform/Window.hpp"

#include "Core/EngineConfig.hpp"
#include "Core/ServiceContainer.hpp"
#include "Platform/Window.hpp"
#include "Systems/Logger.hpp"

namespace GameLibrary
{
    void Window::Init(ServiceContainer& container)
    {
        auto* engineConfig = container.Resolve<EngineConfig>();
        m_width = engineConfig->screenWidth;
        m_height = engineConfig->screenHeight;

        m_window.create(sf::VideoMode(sf::Vector2u(static_cast<unsigned>(m_width), static_cast<unsigned>(m_height))),
                        engineConfig->title, sf::Style::Titlebar | sf::Style::Close);

        if (!m_window.isOpen())
        {
            Logger::Error("Failed to create window!");
            return;
        }

        m_window.setFramerateLimit(60);
    }

    void Window::Destroy() noexcept
    {
        if (m_window.isOpen())
        {
            m_window.close();
        }
    }

    bool Window::ProcessMessages() noexcept
    {
        while (const std::optional event = m_window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                return false;
        }

        return m_window.isOpen();
    }
} // namespace GameLibrary