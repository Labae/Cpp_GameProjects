#pragma once

#include <array>
#include "Interfaces/IInputProvider.hpp"

namespace GameLibrary
{
    class InputSystem : public IInputProvider
    {
    public:
        void Update() noexcept;

        [[nodiscard]] bool IsKeyDown(KeyCode key) const noexcept override;
        [[nodiscard]] bool IsKeyPressed(KeyCode key) const noexcept override;
        [[nodiscard]] bool IsKeyReleased(KeyCode key) const noexcept override;

    private:
        [[nodiscard]] static int32_t ToVirtualKey(KeyCode key) noexcept;

        static constexpr size_t KEY_COUNT = 256;
        std::array<bool, KEY_COUNT> m_currentState = {};
        std::array<bool, KEY_COUNT> m_previousState = {};
    };
}