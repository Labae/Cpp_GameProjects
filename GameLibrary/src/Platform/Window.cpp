#include "Platform/Window.hpp"

#include "../../include/Systems/Logger.hpp"
#include "Core/EngineConfig.hpp"
#include "Core/ServiceContainer.hpp"
#include "Interfaces/IInputProvider.hpp"

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

        // 인터페이스로 Resolve
        m_inputProvider = container.Resolve<IInputProvider>();
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
            if (event->is<sf::Event::Closed>())
                return false;

            if (m_inputProvider)
            {
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    m_inputProvider->OnKeyPressed(keyPressed->scancode);
                }
                if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
                {
                    m_inputProvider->OnKeyReleased(keyReleased->scancode);
                }
            }
        }

        return m_window.isOpen();
    }
} // namespace GameLibrary