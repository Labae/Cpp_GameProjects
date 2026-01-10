#pragma once

#include "Interfaces/IGraphics.hpp"

#include <SFML/Graphics/Color.hpp>
#include <string>
#include <unordered_map>
#include <variant>

namespace GameLibrary
{
    class ConfigService
    {
    public:
        ConfigService() = default;
        ~ConfigService() = default;

        ConfigService(const ConfigService&) = delete;
        ConfigService(ConfigService&&) = delete;
        ConfigService& operator=(const ConfigService&) = delete;
        ConfigService& operator=(ConfigService&&) = delete;

        // 파일 로드 (기존 값에 추가/덮어쓰기)
        [[nodiscard]] bool LoadFromFile(const std::string& path);

        // 모든 값 제거
        void Clear();

        // 값 가져오기 (없으면 기본값 반환)
        [[nodiscard]] int32_t GetInt(const std::string& key, int32_t defaultValue = 0) const;
        [[nodiscard]] float GetFloat(const std::string& key, float defaultValue = 0.0f) const;
        [[nodiscard]] std::string GetString(const std::string& key, const std::string& defaultValue = "") const;
        [[nodiscard]] bool GetBool(const std::string& key, bool defaultValue = false) const;
        [[nodiscard]] sf::Color GetColor(const std::string& key,
                                         const sf::Color& defaultValue = sf::Color::White) const;

        // 키 존재 여부
        [[nodiscard]] bool HasKey(const std::string& key) const;

    private:
        void ParseLine(const std::string& line);

        using Value = std::variant<int32_t, float, bool, std::string>;
        std::unordered_map<std::string, Value> m_values{};
    };
} // namespace GameLibrary