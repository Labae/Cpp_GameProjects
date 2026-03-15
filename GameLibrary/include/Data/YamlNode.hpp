//
// Created by Labae on 3/15/26.
//

#ifndef YAMLNODE_HPP
#define YAMLNODE_HPP

#include <string>
#include <unordered_map>
#include <vector>

namespace GameLibrary
{
    class YamlNode final
    {
    public:
        // 노드가 될 수 있는 타입
        enum class EType : uint8_t
        {
            Null,
            Scalar,
            List,
            Map
        };

        YamlNode() = default;
        ~YamlNode() = default;

        YamlNode(const YamlNode& other) = default;
        YamlNode(YamlNode&& other) = default;
        YamlNode& operator=(const YamlNode& other) = default;
        YamlNode& operator=(YamlNode&& other) = default;

        static YamlNode CreateScalar(const std::string& value);
        static YamlNode CreateList();
        static YamlNode CreateMap();

        [[nodiscard]] const YamlNode& operator[](const std::string& key) const;
        [[nodiscard]] const YamlNode& operator[](size_t index) const;
        [[nodiscard]] const std::vector<YamlNode>& AsList() const;

        [[nodiscard]] std::string AsString(const std::string& defaultValue = "") const;
        [[nodiscard]] int32_t AsInt(int32_t defaultValue = 0) const;
        [[nodiscard]] float AsFloat(float defaultValue = 0.0f) const;
        [[nodiscard]] bool AsBool(bool defaultValue = false) const;

        [[nodiscard]] EType GetType() const noexcept { return m_type; }
        [[nodiscard]] bool IsNull() const noexcept { return m_type == EType::Null; }
        [[nodiscard]] bool IsScalar() const noexcept { return m_type == EType::Scalar; }
        [[nodiscard]] bool IsList() const noexcept { return m_type == EType::List; }
        [[nodiscard]] bool IsMap() const noexcept { return m_type == EType::Map; }

        [[nodiscard]] int32_t Size() const noexcept;

        void AddChild(const std::string& key, YamlNode node);
        void AddChild(YamlNode node);

    private:
        EType m_type{EType::Null};

        std::string m_scalar{};
        std::vector<YamlNode> m_list{};
        std::unordered_map<std::string, YamlNode> m_map{};
    };
} // namespace GameLibrary

#endif // YAMLNODE_HPP
