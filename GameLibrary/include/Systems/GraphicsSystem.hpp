#pragma once

#include "Interfaces/IGraphics.hpp"
#include "Interfaces/IInitializable.hpp"

#include <SFML/Graphics.hpp>

namespace GameLibrary
{
    class Window;
    class ServiceContainer;

    class GraphicsSystem : public IGraphics, public IInitializable
    {
    public:
        GraphicsSystem() = default;
        ~GraphicsSystem() = default;

        GraphicsSystem(const GraphicsSystem&) = delete;
        GraphicsSystem(GraphicsSystem&&) = delete;
        GraphicsSystem& operator=(const GraphicsSystem&) = delete;
        GraphicsSystem& operator=(GraphicsSystem&&) = delete;

        void Init(ServiceContainer& container) override;

        void Clear(const sf::Color& color) noexcept override;
        void DrawRect(int32_t x, int32_t y, int32_t width, int32_t height, const sf::Color& color) noexcept override;
        void FillRect(int32_t x, int32_t y, int32_t width, int32_t height, const sf::Color& color) noexcept override;
        void DrawLabel(const std::string& text, int32_t x, int32_t y, int32_t fontSize, const sf::Color& color,
                       TextAlign align = TextAlign::Left) noexcept override;
        void Present() noexcept override;

        void SetOffset(int32_t x, int32_t y) noexcept override
        {
            m_offsetX = x;
            m_offsetY = y;
        }
        void ResetOffset() noexcept override
        {
            m_offsetX = 0;
            m_offsetY = 0;
        }

    private:
        sf::RenderWindow* m_window;
        sf::Font m_font{};
        bool m_fontLoaded{};

        int32_t m_offsetX{};
        int32_t m_offsetY{};
    };
} // namespace GameLibrary