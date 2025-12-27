#pragma once

#include <string>
#include <unordered_map>
#include "Interfaces/IGraphics.hpp"
#include <variant>

namespace GameLibrary
{
    class ConfigSystem
    {
    public:
        ConfigSystem() = default;
        ~ConfigSystem() = default;

        // 파일 로드 (기존 값에 추가/덮어쓰기)
        [[nodiscard]] bool LoadFromFile(const std::string& path);

        // 모든 값 제거
        void Clear();

        // 값 가져오기 (없으면 기본값 반환)
        [[nodiscard]] int32_t GetInt(const std::string& key, int32_t defaultValue = 0) const;
        [[nodiscard]] float GetFloat(const std::string& key, float defaultValue = 0.0f) const;
        [[nodiscard]] std::string GetString(const std::string& key, const std::string& defaultValue = "") const;
        [[nodiscard]] bool GetBool(const std::string& key, bool defaultValue = false) const;
        [[nodiscard]] Color GetColor(const std::string& key, const Color& defaultValue = {}) const;

        // 키 존재 여부
        [[nodiscard]] bool HasKey(const std::string& key) const;

    private:
        void ParseLine(const std::string& line);

        using Value = std::variant<int32_t, float, bool, std::string>;
        std::unordered_map<std::string, Value> m_values;
    };
}