#pragma once

#include <Windows.h>
#include "Interfaces/IGraphics.hpp"

namespace GameLibrary
{
    class GraphicsSystem : public IGraphics
    {
    public:
        GraphicsSystem() = default;
        ~GraphicsSystem();

        GraphicsSystem(const GraphicsSystem&) = delete;
        GraphicsSystem(GraphicsSystem&&) = delete;
        GraphicsSystem& operator=(const GraphicsSystem&) = delete;
        GraphicsSystem& operator=(GraphicsSystem&&) = delete;

        [[nodiscard]] bool Init(HWND hWnd, int32_t width, int32_t height) noexcept;
        void Shutdown() noexcept;

        void Clear(const Color& color) noexcept override;
        void DrawRect(int32_t x, int32_t y, int32_t width, int32_t height, const Color& color) noexcept override;
        void FillRect(int32_t x, int32_t y, int32_t width, int32_t height, const Color& color) noexcept override;
        void DrawLabel(const wchar_t* text, int32_t x, int32_t y, int32_t width, int32_t fontSize, const Color& color, TextAlign align = TextAlign::Left) noexcept override;
        void Present() noexcept override;

        void SetOffset(int32_t x, int32_t y) noexcept override { m_offsetX = x; m_offsetY = y; }
        void ResetOffset() noexcept override { m_offsetX = 0; m_offsetY = 0; }

    private:
        HWND m_hWnd = nullptr;
        HDC m_hdc = nullptr;
        HDC m_memDC = nullptr;
        HBITMAP m_memBitmap = nullptr;
        HBITMAP m_oldBitmap = nullptr;

        int32_t m_width{};
        int32_t m_height{};
        int32_t m_offsetX{};
        int32_t m_offsetY{};
    };
}