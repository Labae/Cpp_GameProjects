#pragma once

#include <SFML/Window/Keyboard.hpp>

namespace GameLibrary
{
    enum class KeyCode
    {
        None = 0,
        Left,
        Right,
        Up,
        Down,
        Space,
        Enter,
        Escape,
        F5
    };

    class IInputProvider
    {
    public:
        virtual ~IInputProvider() = default;

        virtual void Update() noexcept = 0;
        [[nodiscard]] virtual bool IsKeyDown(KeyCode key) const noexcept = 0;
        [[nodiscard]] virtual bool IsKeyPressed(KeyCode key) const noexcept = 0;
        [[nodiscard]] virtual bool IsKeyReleased(KeyCode key) const noexcept = 0;

        virtual void OnKeyPressed(sf::Keyboard::Scancode scancode) noexcept = 0;
        virtual void OnKeyReleased(sf::Keyboard::Scancode scancode) noexcept = 0;
    };
} // namespace GameLibrary