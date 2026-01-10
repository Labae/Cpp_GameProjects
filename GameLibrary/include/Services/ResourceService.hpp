#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <memory>
#include <string>

namespace GameLibrary
{
    class ResourceService
    {
    public:
        ResourceService() = default;
        ~ResourceService() = default;

        ResourceService(const ResourceService&) = delete;
        ResourceService(ResourceService&&) = delete;
        ResourceService& operator=(const ResourceService&) = delete;
        ResourceService& operator=(ResourceService&&) = delete;

        // Font
        [[nodiscard]] bool LoadFont(const std::string& id, const std::string& path);
        [[nodiscard]] sf::Font& GetFont(const std::string& id) const noexcept;

        // Sound
        [[nodiscard]] bool LoadSound(const std::string& id, const std::string& path);
        [[nodiscard]] sf::SoundBuffer& GetSound(const std::string& id) const noexcept;

        void UnloadAll() noexcept;

    private:
        std::unordered_map<std::string, std::unique_ptr<sf::Font>> m_fonts{};
        std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> m_sounds{};
    };
} // namespace GameLibrary