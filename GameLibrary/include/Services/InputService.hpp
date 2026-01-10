#pragma once

#include "Interfaces/IInputProvider.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <array>
#include <vector>

namespace GameLibrary
{
    class InputService final : public IInputProvider
    {
    public:
        InputService() = default;
        ~InputService() override = default;

        InputService(const InputService&) = delete;
        InputService(InputService&&) = delete;
        InputService& operator=(const InputService&) = delete;
        InputService& operator=(InputService&&) = delete;

        void Update() noexcept override;

        void OnKeyPressed(sf::Keyboard::Scancode scancode) noexcept override;
        void OnKeyReleased(sf::Keyboard::Scancode scancode) noexcept override;

        [[nodiscard]] bool IsKeyDown(KeyCode key) const noexcept override;
        [[nodiscard]] bool IsKeyPressed(KeyCode key) const noexcept override;
        [[nodiscard]] bool IsKeyReleased(KeyCode key) const noexcept override;

    private:
        [[nodiscard]] static sf::Keyboard::Scancode ToScancode(KeyCode key) noexcept;

        static constexpr size_t KEY_COUNT = sf::Keyboard::ScancodeCount;
        std::array<bool, KEY_COUNT> m_currentState{};
        std::array<bool, KEY_COUNT> m_previousState{};

        // 이벤트 큐
        std::vector<sf::Keyboard::Scancode> m_pressedThisFrame{};
        std::vector<sf::Keyboard::Scancode> m_releasedThisFrame{};
    };
} // namespace GameLibrary