#include "InputSystem.hpp"
#include <Windows.h>

namespace GameLibrary
{
	void InputSystem::Update() noexcept
	{
		m_previousState = m_currentState;

		for (size_t i = 0; i < KEY_COUNT; ++i)
		{
			m_currentState[i] = (GetAsyncKeyState(static_cast<int>(i)) & 0x8000) != 0;
		}
	}

	bool InputSystem::IsKeyDown(KeyCode key) const noexcept
	{
		auto vk = ToVirtualKey(key);
		return vk >= 0 && m_currentState[vk];
	}

	bool InputSystem::IsKeyPressed(KeyCode key) const noexcept
	{
		auto vk = ToVirtualKey(key);
		return vk >= 0 && m_currentState[vk] && !m_previousState[vk];
	}

	bool InputSystem::IsKeyReleased(KeyCode key) const noexcept
	{
		auto vk = ToVirtualKey(key);
		return vk >= 0 && !m_currentState[vk] && m_previousState[vk];
	}

	int InputSystem::ToVirtualKey(KeyCode key) noexcept
	{
		switch (key)
		{
		case KeyCode::Left:   return VK_LEFT;
		case KeyCode::Right:  return VK_RIGHT;
		case KeyCode::Up:     return VK_UP;
		case KeyCode::Down:   return VK_DOWN;
		case KeyCode::Space:  return VK_SPACE;
		case KeyCode::Enter:  return VK_RETURN;
		case KeyCode::Escape: return VK_ESCAPE;
		case KeyCode::F5:     return VK_F5;
		default:              return -1;
		}
	}
}