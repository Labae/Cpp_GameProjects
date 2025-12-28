#include "Systems/ConfigSystem.hpp"

#include "Systems/Logger.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>

namespace GameLibrary
{
    bool ConfigSystem::LoadFromFile(const std::string& path)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            Logger::Warning("Failed to open config file: " + path);
            return false;
        }

        m_values.clear();
        std::string line;
        while (std::getline(file, line))
        {
            ParseLine(line);
        }

        return true;
    }

    void ConfigSystem::Clear()
    {
        m_values.clear();
    }

    int32_t ConfigSystem::GetInt(const std::string& key, int32_t defaultValue) const
    {
        auto it = m_values.find(key);
        if (it == m_values.end())
        {
            return defaultValue;
        }

        if (auto* v = std::get_if<int32_t>(&it->second))
        {
            return *v;
        }
        else
        {
            return defaultValue;
        }
    }

    float ConfigSystem::GetFloat(const std::string& key, float defaultValue) const
    {
        auto it = m_values.find(key);
        if (it == m_values.end())
        {
            return defaultValue;
        }

        if (auto* v = std::get_if<float>(&it->second))
        {
            return *v;
        }
        else if (auto* v = std::get_if<int32_t>(&it->second))
        {
            return static_cast<float>(*v);
        }
        else
        {
            return defaultValue;
        }
    }

    std::string ConfigSystem::GetString(const std::string& key, const std::string& defaultValue) const
    {
        auto it = m_values.find(key);
        if (it == m_values.end())
        {
            return defaultValue;
        }

        if (auto* v = std::get_if<std::string>(&it->second))
        {
            return *v;
        }
        else
        {
            return defaultValue;
        }
    }

    bool ConfigSystem::GetBool(const std::string& key, bool defaultValue) const
    {
        auto it = m_values.find(key);
        if (it == m_values.end())
        {
            return defaultValue;
        }

        if (auto* v = std::get_if<bool>(&it->second))
        {
            return *v;
        }
        else if (auto* v = std::get_if<int32_t>(&it->second))
        {
            return *v != 0;
        }
        else
        {
            return defaultValue;
        }
    }

    sf::Color ConfigSystem::GetColor(const std::string& key, const sf::Color& defaultValue) const
    {
        std::string value = GetString(key, "");
        if (value.empty())
            return defaultValue;

        sf::Color color = defaultValue;
        std::stringstream ss(value);
        std::string token;
        size_t index = 0;

        // RGBA 최대값 4개
        while (std::getline(ss, token, ',') && index < 4)
        {
            try
            {
                int32_t v = std::stoi(token);
                v = std::clamp(v, 0, 255);
                switch (index)
                {
                case 0:
                    color.r = static_cast<uint8_t>(v);
                    break;
                case 1:
                    color.g = static_cast<uint8_t>(v);
                    break;
                case 2:
                    color.b = static_cast<uint8_t>(v);
                    break;
                case 3:
                    color.a = static_cast<uint8_t>(v);
                    break;
                }
            }
            catch (...)
            {
                Logger::Warning("Invalid color: " + token);
            }
            ++index;
        }

        return color;
    }

    bool ConfigSystem::HasKey(const std::string& key) const
    {
        return m_values.contains(key);
    }

    void ConfigSystem::ParseLine(const std::string& line)
    {
        // 주석 제거
        size_t commentPos = line.find('#');
        std::string content = (commentPos != std::string::npos) ? line.substr(0, commentPos) : line;

        // 공백 제거
        auto start = content.find_first_not_of(" \t");
        if (start == std::string::npos)
        {
            return;
        }

        auto colonPos = content.find(':');
        if (colonPos == std::string::npos)
        {
            return;
        }

        std::string key = content.substr(start, colonPos - start);
        std::string value = content.substr(colonPos + 1);

        // key 공백 제거
        key.erase(key.find_last_not_of(" \t") + 1);

        // value 앞뒤 공백 제거
        auto valueStart = value.find_first_not_of(" \t");
        if (valueStart == std::string::npos)
        {
            return;
        }
        value = value.substr(valueStart);
        value.erase(value.find_last_not_of(" \t") + 1);

        // 소문자로 모두 변경
        std::string lowerValue = value;
        std::transform(lowerValue.begin(), lowerValue.end(), lowerValue.begin(), ::tolower);

        if (lowerValue == "true" || lowerValue == "yes" || lowerValue == "on")
        {
            m_values[key] = true;
        }
        else if (lowerValue == "false" || lowerValue == "no" || lowerValue == "off")
        {
            m_values[key] = false;
        }
        // 쉼표가 있으면 string (Color, ...)
        else if (value.find(',') != std::string::npos)
        {
            m_values[key] = value;
        }
        else if (value.find('.') != std::string::npos)
        {
            try
            {
                m_values[key] = std::stof(value);
            }
            catch (...)
            {
                m_values[key] = value;
            }
        }
        else
        {
            try
            {
                m_values[key] = std::stoi(value);
            }
            catch (...)
            {
                m_values[key] = value;
            }
        }
    }
} // namespace GameLibrary