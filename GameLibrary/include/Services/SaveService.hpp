#pragma once

#include <string>
#include <unordered_map>
#include <variant>

namespace GameLibrary
{
    class SaveService
    {
    public:
        SaveService() = default;
        ~SaveService() = default;

        SaveService(const SaveService&) = delete;
        SaveService(SaveService&&) = delete;
        SaveService& operator=(const SaveService&) = delete;
        SaveService& operator=(SaveService&&) = delete;

        // 값 설정
        void SetInt(const std::string& key, int32_t value);
        void SetFloat(const std::string& key, float value);
        void SetString(const std::string& key, const std::string& value);

        // 값 가져오기 (없으면 기본값 반환)
        [[nodiscard]] int GetInt(const std::string& key, int32_t defaultValue = 0) const;
        [[nodiscard]] float GetFloat(const std::string& key, float defaultValue = 0.0f) const;
        [[nodiscard]] std::string GetString(const std::string& key, const std::string& defaultValue = "") const;

        // 키 존재 여부
        [[nodiscard]] bool HasKey(const std::string& key) const;
        void Remove(const std::string& key);
        void Clear();

        // 파일 I/O
        [[nodiscard]] bool LoadFromFile(const std::string& path);
        [[nodiscard]] bool SaveToFile(const std::string& path) const;

    private:
        enum class ValueType : uint8_t
        {
            Int = 0,
            Float = 1,
            String = 2
        };

        using Value = std::variant<int, float, std::string>;
        std::unordered_map<std::string, Value> m_data{};
    };
} // namespace GameLibrary