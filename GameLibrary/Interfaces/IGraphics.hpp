#pragma once

#include <cstdint>

namespace GameLibrary
{
    struct Color
    {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t a = 255;
    };

    enum class TextAlign
    {
        Left,
        Center,
        Right
    };

    class IGraphics
    {
    public:
        virtual ~IGraphics() = default;

        virtual void Clear(const Color& color) = 0;
        virtual void DrawRect(int32_t x, int32_t y, int32_t width, int32_t height, const Color& color) = 0;
        virtual void FillRect(int32_t x, int32_t y, int32_t width, int32_t height, const Color& color) = 0;
        virtual void DrawLabel(const wchar_t* text, int32_t x, int32_t y, int32_t width, int32_t fontSize, const Color& color, TextAlign align = TextAlign::Left) = 0;
        virtual void Present() = 0;

        // 오프셋 (화면 흔들림)
        virtual void SetOffset(int32_t x, int32_t y) = 0;
        virtual void ResetOffset() = 0;
    };
}