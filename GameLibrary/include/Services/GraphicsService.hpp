#pragma once

#include "Interfaces/IGraphics.hpp"
#include "Interfaces/IInitializable.hpp"

#include <SFML/Graphics.hpp>

namespace GameLibrary
{
    class ResourceService;
}
namespace GameLibrary
{
    class Window;
    class ServiceContainer;

    class GraphicsService final : public IGraphics, public IInitializable
    {
    public:
        GraphicsService() = default;
        ~GraphicsService() override = default;

        GraphicsService(const GraphicsService&) = delete;
        GraphicsService(GraphicsService&&) = delete;
        GraphicsService& operator=(const GraphicsService&) = delete;
        GraphicsService& operator=(GraphicsService&&) = delete;

        void Init(ServiceContainer& container) override;

        void Clear(const sf::Color& color) noexcept override;
        void DrawRect(int32_t x, int32_t y, int32_t width, int32_t height, const sf::Color& color) noexcept override;
        void FillRect(int32_t x, int32_t y, int32_t width, int32_t height, const sf::Color& color) noexcept override;
        void DrawLabel(const std::string& text, int32_t x, int32_t y, int32_t fontSize, const sf::Color& color,
                       TextAlign align = TextAlign::Left) noexcept override ;
        void Present() noexcept override;

        void SetOffset(const int32_t x, const int32_t y) noexcept override
        {
            m_offsetX = x;
            m_offsetY = y;
        }

        void ResetOffset() noexcept override
        {
            SetOffset(0, 0);
        }

    private:
        sf::RenderWindow* m_window{};
        ResourceService* m_resourceService{};

        int32_t m_offsetX{};
        int32_t m_offsetY{};
    };
} // namespace GameLibrary