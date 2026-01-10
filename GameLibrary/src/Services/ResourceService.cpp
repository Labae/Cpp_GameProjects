//
// Created by Labae on 1/10/26.
//

#include "Services/ResourceService.hpp"

#include "../../include/Systems/Logger.hpp"

namespace GameLibrary
{
    bool ResourceService::LoadFont(const std::string& id, const std::string& path)
    {
        if (m_fonts.contains(id))
        {
            return true;
        }

        auto font = std::make_unique<sf::Font>();
        if (!font->openFromFile(std::string{path}))
        {
            Logger::Warning("Failed to load font: " + std::string{path});
            return false;
        }

        m_fonts[id] = std::move(font);
        Logger::Info("Loaded font: " + id);
        return true;
    }

    sf::Font& ResourceService::GetFont(const std::string& id) const noexcept
    {
        const auto iter = m_fonts.find(id);
        assert(iter != m_fonts.end() && "Font not loaded");
        return *iter->second;
    }

    bool ResourceService::LoadSound(const std::string& id, const std::string& path)
    {
        if (m_sounds.contains(id))
        {
            return true;
        }

        auto sound = std::make_unique<sf::SoundBuffer>();
        if (!sound->loadFromFile(std::string{path}))
        {
            Logger::Warning("Failed to load sound: " + std::string{path});
            return false;
        }

        m_sounds[id] = std::move(sound);
        Logger::Info("Loaded sound: " + id);
        return true;
    }

    sf::SoundBuffer& ResourceService::GetSound(const std::string& id) const noexcept
    {
        const auto iter = m_sounds.find(id);
        assert(iter != m_sounds.end() && "Sound not loaded");
        return *iter->second;
    }

    void ResourceService::UnloadAll() noexcept
    {
        m_fonts.clear();
        m_sounds.clear();
        Logger::Info("Unloading all resources");
    }
} // namespace GameLibrary