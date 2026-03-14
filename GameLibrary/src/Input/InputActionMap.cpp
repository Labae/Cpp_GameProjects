#include "Input/InputActionMap.hpp"

#include <algorithm>

namespace GameLibrary
{
    InputActionMap::InputActionMap(IInputProvider& input) noexcept : m_input(input) {}

    void InputActionMap::Map(const std::string& action, const KeyCode key)
    {
        if (auto& keys = m_bindings[action]; std::ranges::find(keys, key) == keys.end())
        {
            keys.emplace_back(key);
        }
    }

    void InputActionMap::Map(const std::string& action, const std::initializer_list<KeyCode> keys)
    {
        for (const auto key : keys)
        {
            Map(action, key);
        }
    }

    void InputActionMap::Unmap(const std::string& action)
    {
        m_bindings.erase(action);
    }

    bool InputActionMap::IsDown(const std::string& action) const noexcept
    {
        const auto it = m_bindings.find(action);
        if (it == m_bindings.end())
        {
            return false;
        }

        return std::ranges::any_of(it->second, [this](const KeyCode key)
        {
            return m_input.IsKeyDown(key);
        });
    }

    bool InputActionMap::IsPressed(const std::string& action) const noexcept
    {
        const auto it = m_bindings.find(action);
        if (it == m_bindings.end())
        {
            return false;
        }

        return std::ranges::any_of(it->second, [this](const KeyCode key)
        {
            return m_input.IsKeyPressed(key);
        });
    }

    bool InputActionMap::IsReleased(const std::string& action) const noexcept
    {
        const auto it = m_bindings.find(action);
        if (it == m_bindings.end())
        {
            return false;
        }

        return std::ranges::any_of(it->second, [this](const KeyCode key)
        {
            return m_input.IsKeyReleased(key);
        });
    }
} // namespace GameLibrary