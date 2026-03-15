//
// Created by Labae on 3/15/26.
//

#include "Data/YamlNode.hpp"

#include "Systems/Logger.hpp"

namespace
{
    const GameLibrary::YamlNode NULL_NODE{};
    constexpr std::vector<GameLibrary::YamlNode> EMPTY_LIST{};
} // namespace

namespace GameLibrary
{
    YamlNode YamlNode::CreateScalar(const std::string& value)
    {
        YamlNode node;
        node.m_type = EType::Scalar;
        node.m_scalar = value;
        return node;
    }

    YamlNode YamlNode::CreateList()
    {
        YamlNode node;
        node.m_type = EType::List;
        return node;
    }

    YamlNode YamlNode::CreateMap()
    {
        YamlNode node;
        node.m_type = EType::Map;
        return node;
    }

    const YamlNode& YamlNode::operator[](const std::string& key) const
    {
        if (m_type != EType::Map)
        {
            return NULL_NODE;
        }

        const auto it = m_map.find(key);
        if (it == m_map.end())
        {
            return NULL_NODE;
        }

        return it->second;
    }

    const YamlNode& YamlNode::operator[](const size_t index) const
    {
        if (m_type != EType::List or index >= m_list.size())
        {
            return NULL_NODE;
        }
        return m_list[index];
    }

    const std::vector<YamlNode>& YamlNode::AsList() const
    {
        if (m_type != EType::List)
        {
            return EMPTY_LIST;
        }
        return m_list;
    }

    std::string YamlNode::AsString(const std::string& defaultValue) const
    {
        if (m_type != EType::Scalar)
        {
            return defaultValue;
        }
        return m_scalar;
    }

    int32_t YamlNode::AsInt(const int32_t defaultValue) const
    {
        if (m_type != EType::Scalar)
        {
            return defaultValue;
        }

        try
        {
            return std::stoi(m_scalar);
        }
        catch (...)
        {
            return defaultValue;
        }
    }

    float YamlNode::AsFloat(const float defaultValue) const
    {
        if (m_type != EType::Scalar)
        {
            return defaultValue;
        }

        try
        {
            return std::stof(m_scalar);
        }
        catch (...)
        {
            return defaultValue;
        }
    }

    bool YamlNode::AsBool(const bool defaultValue) const
    {
        if (m_type != EType::Scalar)
        {
            return defaultValue;
        }

        if (m_scalar == "true" or m_scalar == "yes" or m_scalar == "on")
        {
            return true;
        }
        if (m_scalar == "false" or m_scalar == "no" or m_scalar == "off")
        {
            return false;
        }

        return defaultValue;
    }

    int32_t YamlNode::Size() const noexcept
    {
        switch (m_type)
        {
        case EType::List:
            return static_cast<int32_t>(m_list.size());
        case EType::Map:
            return static_cast<int32_t>(m_map.size());
        default:
            return 0;
        }
    }

    void YamlNode::AddChild(const std::string& key, YamlNode node)
    {
        if (m_type != EType::Map and m_type != EType::Null)
        {
            Logger::Error("AddChild(key): expected Map, got " + std::to_string(static_cast<int>(m_type)));
            return;
        }
        m_type = EType::Map;
        m_map[key] = std::move(node);
    }

    void YamlNode::AddChild(YamlNode node)
    {
        if (m_type != EType::List and m_type != EType::Null)
        {
            Logger::Error("AddChild: expected List, got " + std::to_string(static_cast<int>(m_type)));
            return;
        }
        m_type = EType::List;
        m_list.emplace_back(std::move(node));
    }
} // namespace GameLibrary