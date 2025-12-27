#include "GraphicsSystem.hpp"
#include <algorithm>

#pragma comment(lib, "Msimg32.lib")

namespace GameLibrary
{
	GraphicsSystem::~GraphicsSystem()
	{
		Shutdown();
	}

	bool GraphicsSystem::Init(HWND hWnd, int32_t width, int32_t height) noexcept
	{
		m_hWnd = hWnd;
		m_width = width;
		m_height = height;

		m_hdc = GetDC(hWnd);
		if (!m_hdc)
		{
			return false;
		}

		m_memDC = CreateCompatibleDC(m_hdc);
		if (!m_memDC)
		{
			return false;
		}

		m_memBitmap = CreateCompatibleBitmap(m_hdc, width, height);
		if (!m_memBitmap)
		{
			return false;
		}

		m_oldBitmap = static_cast<HBITMAP>(SelectObject(m_memDC, m_memBitmap));

		return true;
	}

	void GraphicsSystem::Shutdown() noexcept
	{
		if (m_memDC && m_oldBitmap)
		{
			SelectObject(m_memDC, m_oldBitmap);
			m_oldBitmap = nullptr;
		}

		if (m_memBitmap)
		{
			DeleteObject(m_memBitmap);
			m_memBitmap = nullptr;
		}

		if (m_memDC)
		{
			DeleteDC(m_memDC);
			m_memDC = nullptr;
		}

		if (m_hWnd && m_hdc)
		{
			ReleaseDC(m_hWnd, m_hdc);
			m_hdc = nullptr;
		}
	}

	void GraphicsSystem::Clear(const Color& color) noexcept
	{
		HBRUSH brush = CreateSolidBrush(RGB(color.r, color.g, color.b));
		RECT rect = { 0, 0, m_width, m_height };
		::FillRect(m_memDC, &rect, brush);
		DeleteObject(brush);
	}

	void GraphicsSystem::DrawRect(int32_t x, int32_t y, int32_t width, int32_t height, const Color& color) noexcept
	{
		x += m_offsetX;
		y += m_offsetY;

		HPEN pen = CreatePen(PS_SOLID, 1, RGB(color.r, color.g, color.b));
		HPEN oldPen = static_cast<HPEN>(SelectObject(m_memDC, pen));
		HBRUSH oldBrush = static_cast<HBRUSH>(SelectObject(m_memDC, GetStockObject(NULL_BRUSH)));

		Rectangle(m_memDC, x, y, x + width, y + height);

		SelectObject(m_memDC, oldBrush);
		SelectObject(m_memDC, oldPen);
		DeleteObject(pen);
	}

	void GraphicsSystem::FillRect(int32_t x, int32_t y, int32_t width, int32_t height, const Color& color) noexcept
	{
		x += m_offsetX;
		y += m_offsetY;

		if (color.a == 255)
		{
			HBRUSH brush = CreateSolidBrush(RGB(color.r, color.g, color.b));
			RECT rect = { x, y, x + width, y + height };
			::FillRect(m_memDC, &rect, brush);
			DeleteObject(brush);
		}
		else
		{
			BITMAPINFO bmi = {};
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = width;
			bmi.bmiHeader.biHeight = -height;
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = 32;
			bmi.bmiHeader.biCompression = BI_RGB;

			void* bits = nullptr;
			HDC tempDC = CreateCompatibleDC(m_memDC);
			HBITMAP tempBitmap = CreateDIBSection(tempDC, &bmi, DIB_RGB_COLORS, &bits, nullptr, 0);

			if (tempBitmap && bits)
			{
				HBITMAP oldBitmap = static_cast<HBITMAP>(SelectObject(tempDC, tempBitmap));
				
				// 픽셀 채우기
				uint32_t* pixels = static_cast<uint32_t*>(bits);
				uint32_t pixelColor = color.b | (color.g << 8) | (color.r << 16) | (0xFF << 24);
				std::fill(pixels, pixels + width * height, pixelColor);


				BLENDFUNCTION blend
				{
					.BlendOp = AC_SRC_OVER,
					.SourceConstantAlpha = color.a
				};
				AlphaBlend(m_memDC, x, y, width, height, tempDC, 0, 0, width, height, blend);
			
				SelectObject(tempDC, oldBitmap);
				DeleteObject(tempBitmap);
			}

			DeleteDC(tempDC);
		}
	}

	void GraphicsSystem::DrawLabel(const wchar_t* text, int32_t x, int32_t y, int32_t width, int32_t fontSize, const Color& color, TextAlign align) noexcept
	{
		HFONT font = CreateFontW(
			fontSize,           // 높이
			0,                  // 너비 (0 = 자동)
			0,                  // 기울기 각도
			0,                  // 방향 각도
			FW_NORMAL,          // 굵기
			FALSE,              // 이탤릭
			FALSE,              // 밑줄
			FALSE,              // 취소선
			DEFAULT_CHARSET,    // 문자셋
			OUT_DEFAULT_PRECIS, // 출력 정밀도
			CLIP_DEFAULT_PRECIS,// 클리핑 정밀도
			CLEARTYPE_QUALITY,  // 품질
			DEFAULT_PITCH | FF_DONTCARE, // 피치 및 패밀리
			L"Consolas"         // 폰트 이름
		);

		HFONT oldFont = static_cast<HFONT>(SelectObject(m_memDC, font));
		SetTextColor(m_memDC, RGB(color.r, color.g, color.b));
		SetBkMode(m_memDC, TRANSPARENT);

		UINT format = DT_SINGLELINE | DT_NOCLIP;
		switch (align)
		{
		case TextAlign::Left:
			format |= DT_LEFT;
			break;
		case TextAlign::Center:
			format |= DT_CENTER;
			break;
		case TextAlign::Right:
			format |= DT_RIGHT;
			break;
		}

		RECT rect = { x, y, x + width, y + fontSize };
		::DrawTextW(m_memDC, text, -1, &rect, format);

		SelectObject(m_memDC, oldFont);
		DeleteObject(font);
	}

	void GraphicsSystem::Present() noexcept
	{
		BitBlt(m_hdc, 0, 0, m_width, m_height, m_memDC, 0, 0, SRCCOPY);
	}
}