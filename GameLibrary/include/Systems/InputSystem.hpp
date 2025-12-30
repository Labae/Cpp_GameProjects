#pragma once

#include "Interfaces/IInputProvider.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <array>
#include <vector>

namespace GameLibrary
{
    class InputSystem : public IInputProvider
    {
    public:
        InputSystem() = default;
        ~InputSystem() = default;

        InputSystem(const InputSystem&) = delete;
        InputSystem(InputSystem&&) = delete;
        InputSystem& operator=(const InputSystem&) = delete;
        InputSystem& operator=(InputSystem&&) = delete;

        void Update() noexcept override;

        void OnKeyPressed(sf::Keyboard::Scancode scancode) noexcept override;
        void OnKeyReleased(sf::Keyboard::Scancode scancode) noexcept override;

        [[nodiscard]] bool IsKeyDown(KeyCode key) const noexcept override;
        [[nodiscard]] bool IsKeyPressed(KeyCode key) const noexcept override;
        [[nodiscard]] bool IsKeyReleased(KeyCode key) const noexcept override;

    private:
        [[nodiscard]] static sf::Keyboard::Scancode ToScancode(KeyCode key) noexcept;

        static constexpr size_t KEY_COUNT = static_cast<size_t>(sf::Keyboard::ScancodeCount);
        std::array<bool, KEY_COUNT> m_currentState{};
        std::array<bool, KEY_COUNT> m_previousState{};

        // 이벤트 큐
        std::vector<sf::Keyboard::Scancode> m_pressedThisFrame{};
        std::vector<sf::Keyboard::Scancode> m_releasedThisFrame{};
    };
} // namespace GameLibrary