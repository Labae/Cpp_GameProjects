#include "SaveSystem.hpp"
#include <fstream>

namespace GameLibrary
{
    void SaveSystem::SetInt(const std::string& key, int32_t value)
    {
        m_data[key] = value;
    }

    void SaveSystem::SetFloat(const std::string& key, float value)
    {
        m_data[key] = value;
    }

    void SaveSystem::SetString(const std::string& key, const std::string& value)
    {
        m_data[key] = value;
    }

    int SaveSystem::GetInt(const std::string& key, int32_t defaultValue) const
    {
        auto it = m_data.find(key);
        if (it == m_data.end())
        {
            return defaultValue;
        }

        if (auto* val = std::get_if<int32_t>(&it->second))
        {
            return *val;
        }
        return defaultValue;
    }

    float SaveSystem::GetFloat(const std::string& key, float defaultValue) const
    {
        auto it = m_data.find(key);
        if (it == m_data.end())
        {
            return defaultValue;
        }

        if (auto* val = std::get_if<float>(&it->second))
        {
            return *val;
        }
        return defaultValue;
    }

    std::string SaveSystem::GetString(const std::string& key, const std::string& defaultValue) const
    {
        auto it = m_data.find(key);
        if (it == m_data.end())
        {
            return defaultValue;
        }

        if (auto* val = std::get_if<std::string>(&it->second))
        {
            return *val;
        }
        return defaultValue;
    }

    bool SaveSystem::HasKey(const std::string& key) const
    {
        return m_data.contains(key);
    }

    void SaveSystem::Remove(const std::string& key)
    {
        m_data.erase(key);
    }

    void SaveSystem::Clear()
    {
        m_data.clear();
    }

    bool SaveSystem::LoadFromFile(const std::string& path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
        {
            return false;
        }

        m_data.clear();

        // 항목 개수 읽기
        size_t count = 0;
        file.read(reinterpret_cast<char*>(&count), sizeof(count));

        for (size_t i = 0; i < count && file.good(); ++i)
        {
            // 키 읽기
            size_t keyLen = 0;
            file.read(reinterpret_cast<char*>(&keyLen), sizeof(keyLen));

            std::string key(keyLen, '\0');
            file.read(key.data(), keyLen);

            // 타입 읽기
            ValueType type{};
            file.read(reinterpret_cast<char*>(&type), sizeof(type));

            // 값 읽기
            switch (type)
            {
            case ValueType::Int:
            {
                int value = 0;
                file.read(reinterpret_cast<char*>(&value), sizeof(value));
                m_data[key] = value;
                break;
            }
            case ValueType::Float:
            {
                float value = 0.0f;
                file.read(reinterpret_cast<char*>(&value), sizeof(value));
                m_data[key] = value;
                break;
            }
            case ValueType::String:
            {
                size_t strLen = 0;
                file.read(reinterpret_cast<char*>(&strLen), sizeof(strLen));

                std::string value(strLen, '\0');
                file.read(value.data(), strLen);
                m_data[key] = value;
                break;
            }
            }
        }

        return file.good() || file.eof();
    }

    bool SaveSystem::SaveToFile(const std::string& path) const
    {
        std::ofstream file(path, std::ios::binary);
        if (!file.is_open())
        {
            return false;
        }

        // 항목 개수 쓰기
        size_t count = m_data.size();
        file.write(reinterpret_cast<const char*>(&count), sizeof(count));

        for (const auto& [key, value] : m_data)
        {
            // 키 쓰기
            size_t keyLen = key.size();
            file.write(reinterpret_cast<const char*>(&keyLen), sizeof(keyLen));
            file.write(key.data(), keyLen);

            // 타입과 값 쓰기
            if (auto* intVal = std::get_if<int>(&value))
            {
                ValueType type = ValueType::Int;
                file.write(reinterpret_cast<const char*>(&type), sizeof(type));
                file.write(reinterpret_cast<const char*>(intVal), sizeof(*intVal));
            }
            else if (auto* floatVal = std::get_if<float>(&value))
            {
                ValueType type = ValueType::Float;
                file.write(reinterpret_cast<const char*>(&type), sizeof(type));
                file.write(reinterpret_cast<const char*>(floatVal), sizeof(*floatVal));
            }
            else if (auto* strVal = std::get_if<std::string>(&value))
            {
                ValueType type = ValueType::String;
                file.write(reinterpret_cast<const char*>(&type), sizeof(type));

                size_t strLen = strVal->size();
                file.write(reinterpret_cast<const char*>(&strLen), sizeof(strLen));
                file.write(strVal->data(), strLen);
            }
        }

        return file.good();
    }
}