#include "Systems/InputSystem.hpp"

namespace GameLibrary
{
    void InputSystem::Update() noexcept
    {
        m_previousState = m_currentState;

        for (size_t i = 0; i < KEY_COUNT; ++i)
        {
            m_currentState[i] = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Scancode>(i));
        }
    }

    bool InputSystem::IsKeyDown(KeyCode key) const noexcept
    {
        const auto scancode = ToScancode(key);
        if (scancode == sf::Keyboard::Scan::Unknown)
        {
            return false;
        }
        return m_currentState[static_cast<size_t>(scancode)];
    }

    bool InputSystem::IsKeyPressed(KeyCode key) const noexcept
    {
        const auto scancode = ToScancode(key);
        if (scancode == sf::Keyboard::Scan::Unknown)
        {
            return false;
        }

        const auto index = static_cast<size_t>(scancode);
        return m_currentState[index] && !m_previousState[index];
    }

    bool InputSystem::IsKeyReleased(KeyCode key) const noexcept
    {
        const auto scancode = ToScancode(key);
        if (scancode == sf::Keyboard::Scan::Unknown)
        {
            return false;
        }

        const auto index = static_cast<size_t>(scancode);
        return !m_currentState[index] && m_previousState[index];
    }

    sf::Keyboard::Scancode InputSystem::ToScancode(KeyCode key) noexcept
    {
        switch (key)
        {
        case KeyCode::Left:
            return sf::Keyboard::Scan::Left;
        case KeyCode::Right:
            return sf::Keyboard::Scan::Right;
        case KeyCode::Up:
            return sf::Keyboard::Scan::Up;
        case KeyCode::Down:
            return sf::Keyboard::Scan::Down;
        case KeyCode::Space:
            return sf::Keyboard::Scan::Space;
        case KeyCode::Enter:
            return sf::Keyboard::Scan::Enter;
        case KeyCode::Escape:
            return sf::Keyboard::Scan::Escape;
        case KeyCode::F5:
            return sf::Keyboard::Scan::F5;
        default:
            return sf::Keyboard::Scan::Unknown;
        }
    }
} // namespace GameLibrary