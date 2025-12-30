#pragma once

#include "Interfaces/IInitializable.hpp"

#include <SFML/Graphics.hpp>
#include <string>

namespace GameLibrary
{
    class IInputProvider;

    class Window : public IInitializable
    {
    public:
        Window() = default;
        ~Window() = default;

        Window(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;

        void Init(ServiceContainer& container) override;
        void Destroy() noexcept;

        [[nodiscard]] bool ProcessMessages() noexcept;

        [[nodiscard]] sf::RenderWindow& GetHandle() noexcept { return m_window; }
        [[nodiscard]] int32_t GetWidth() const noexcept { return m_width; }
        [[nodiscard]] int32_t GetHeight() const noexcept { return m_height; }

    private:
        sf::RenderWindow m_window{};
        IInputProvider* m_inputProvider{};
        int32_t m_width{};
        int32_t m_height{};
    };
} // namespace GameLibrary