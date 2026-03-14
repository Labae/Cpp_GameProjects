#ifndef INPUTACTIONMAP_HPP
#define INPUTACTIONMAP_HPP

#include "Interfaces/IInputProvider.hpp"

#include <initializer_list>
#include <string>
#include <unordered_map>
#include <vector>

namespace GameLibrary
{
    class InputActionMap
    {
    public:
        explicit InputActionMap(IInputProvider& input) noexcept;
        ~InputActionMap() = default;

        InputActionMap(const InputActionMap&) = delete;
        InputActionMap(InputActionMap&&) = default;
        InputActionMap& operator=(const InputActionMap&) = delete;
        InputActionMap& operator=(InputActionMap&&) = delete;

        void Map(const std::string& action, KeyCode key);
        void Map(const std::string& action, std::initializer_list<KeyCode> keys);
        void Unmap(const std::string& action);

        [[nodiscard]] bool IsDown(const std::string& action) const noexcept;
        [[nodiscard]] bool IsPressed(const std::string& action) const noexcept;
        [[nodiscard]] bool IsReleased(const std::string& action) const noexcept;

    private:
        IInputProvider& m_input;
        std::unordered_map<std::string, std::vector<KeyCode>> m_bindings{};
    };
} // namespace GameLibrary

#endif // INPUTACTIONMAP_HPP