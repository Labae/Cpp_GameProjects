#include "../../include/Services/InputService.hpp"

namespace GameLibrary
{
    void InputService::Update() noexcept
    {
        m_previousState = m_currentState;

        // 이번 프레임에 눌린 키들 처리
        for (auto scancode : m_pressedThisFrame)
        {
            m_currentState[static_cast<size_t>(scancode)] = true;
        }
        for (auto scancode : m_releasedThisFrame)
        {
            m_currentState[static_cast<size_t>(scancode)] = false;
        }

        m_pressedThisFrame.clear();
        m_releasedThisFrame.clear();
    }

    void InputService::OnKeyPressed(sf::Keyboard::Scancode scancode) noexcept
    {
        m_pressedThisFrame.push_back(scancode);
    }

    void InputService::OnKeyReleased(sf::Keyboard::Scancode scancode) noexcept
    {
        m_releasedThisFrame.push_back(scancode);
    }

    bool InputService::IsKeyDown(KeyCode key) const noexcept
    {
        const auto scancode = ToScancode(key);
        if (scancode == sf::Keyboard::Scan::Unknown)
        {
            return false;
        }
        return m_currentState[static_cast<size_t>(scancode)];
    }

    bool InputService::IsKeyPressed(KeyCode key) const noexcept
    {
        const auto scancode = ToScancode(key);
        if (scancode == sf::Keyboard::Scan::Unknown)
        {
            return false;
        }

        const auto index = static_cast<size_t>(scancode);
        return m_currentState[index] && !m_previousState[index];
    }

    bool InputService::IsKeyReleased(KeyCode key) const noexcept
    {
        const auto scancode = ToScancode(key);
        if (scancode == sf::Keyboard::Scan::Unknown)
        {
            return false;
        }

        const auto index = static_cast<size_t>(scancode);
        return !m_currentState[index] && m_previousState[index];
    }

    sf::Keyboard::Scancode InputService::ToScancode(KeyCode key) noexcept
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