#pragma once

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

        virtual bool IsKeyDown(KeyCode key) const = 0;
        virtual bool IsKeyPressed(KeyCode key) const = 0;
        virtual bool IsKeyReleased(KeyCode key) const = 0;
    };
}