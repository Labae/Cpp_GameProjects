#pragma once

#include <Windows.h>
#include <functional>

namespace GameLibrary
{
    class Window
    {
    public:
        Window() = default;
        ~Window();

        Window(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;

        [[nodiscard]] bool Create(HINSTANCE hInstance, int32_t width, int32_t height, const wchar_t* title) noexcept;
        void Destroy() noexcept;

        [[nodiscard]] bool ProcessMessages() noexcept;

        [[nodiscard]] HWND GetHandle() const noexcept { return m_hWnd; }
        [[nodiscard]] int32_t GetWidth() const noexcept { return m_width; }
        [[nodiscard]] int32_t GetHeight() const noexcept { return m_height; }

    private:
        static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

        HWND m_hWnd = nullptr;
        HINSTANCE m_hInstance = nullptr;
        int32_t m_width = 0;
        int32_t m_height = 0;

        static constexpr const wchar_t* CLASS_NAME = L"GameLibraryWindowClass";
    };
}