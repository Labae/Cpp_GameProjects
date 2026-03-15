#include "Data/YamlParser.hpp"

#include "Systems/Logger.hpp"

#include <fstream>
#include <sstream>

namespace GameLibrary
{
    YamlNode YamlParser::LoadFromFile(const std::string& path)
    {
        std::ifstream file(path);
        if (not file.is_open())
        {
            Logger::Warning("Failed to open YAML file: " + path);
            return {};
        }

        std::ostringstream ss;
        ss << file.rdbuf();
        return LoadFromString(ss.str());
    }

    YamlNode YamlParser::LoadFromString(const std::string& content)
    {
        const auto lines = Tokenize(content);
        if (lines.empty())
        {
            return {};
        }

        size_t index = 0;
        return ParseNode(lines, index);
    }

    std::vector<YamlParser::Line> YamlParser::Tokenize(const std::string& content)
    {
        std::vector<Line> lines;
        std::istringstream stream(content);
        std::string raw;

        while (std::getline(stream, raw))
        {
            // 주석 제거
            if (const auto commentPos = raw.find('#'); commentPos != std::string::npos)
            {
                raw = raw.substr(0, commentPos);
            }

            // 빈 줄 스킵
            if (raw.find_first_not_of(" \t") == std::string::npos)
            {
                continue;
            }

            // 인덴트 계산
            int32_t indent = 0;
            for (const char c : raw)
            {
                if (c == ' ')
                {
                    ++indent;
                }
                else
                {
                    break;
                }
            }

            // 내용 추출 + 뒤쪽 공백 제거
            std::string lineContent = raw.substr(indent);
            if (const auto lastNonSpace = lineContent.find_last_not_of(" \t");
                lastNonSpace != std::string::npos)
            {
                lineContent = lineContent.substr(0, lastNonSpace + 1);
            }

            lines.push_back({indent, std::move(lineContent)});
        }

        return lines;
    }

    YamlNode YamlParser::ParseNode(const std::vector<Line>& lines, size_t& index)
    {
        if (index >= lines.size())
        {
            return {};
        }

        // ===== 디버그 출력 (나중에 제거) =====
        std::cout << "[ParseNode] index=" << index
                  << " indent=" << lines[index].indent
                  << " content=\"" << lines[index].content << "\""
                  << " starts_with_dash=" << lines[index].content.starts_with("- ")
                  << "\n";
        // =====================================

        const int32_t currentIndent = lines[index].indent;

        // ===== 리스트 파싱 =====
        if (lines[index].content.starts_with("- "))
        {
            auto listNode = YamlNode::CreateList();

            while (index < lines.size()
                   and lines[index].indent == currentIndent
                   and lines[index].content.starts_with("- "))
            {
                const std::string afterDash = Trim(lines[index].content.substr(2));

                if (const auto colonPos = afterDash.find(':'); colonPos == std::string::npos)
                {
                    // 케이스 A: 스칼라 — "- hp_potion"
                    listNode.AddChild(ParseScalar(afterDash));
                    ++index;
                }
                else
                {
                    // 케이스 B: 오브젝트 — "- id: goblin"
                    auto mapNode = YamlNode::CreateMap();

                    const std::string firstKey = Trim(afterDash.substr(0, colonPos));
                    const std::string firstValue = Trim(afterDash.substr(colonPos + 1));

                    if (not firstValue.empty())
                    {
                        mapNode.AddChild(firstKey, ParseScalar(firstValue));
                    }

                    ++index;

                    // 첫 키의 value가 비어있으면 자식 파싱
                    if (firstValue.empty()
                        and index < lines.size()
                        and lines[index].indent > currentIndent)
                    {
                        auto child = ParseNode(lines, index);
                        mapNode.AddChild(firstKey, std::move(child));
                    }

                    // 나머지 키-값 쌍 — 인덴트가 더 깊고 "- "가 아닌 줄들
                    while (index < lines.size()
                           and lines[index].indent > currentIndent
                           and not lines[index].content.starts_with("- "))
                    {
                        const auto& [indent, content] = lines[index];
                        const auto subColonPos = content.find(':');

                        if (subColonPos == std::string::npos)
                        {
                            ++index;
                            continue;
                        }

                        const std::string subKey = Trim(content.substr(0, subColonPos));
                        const std::string subValue = Trim(content.substr(subColonPos + 1));

                        if (not subValue.empty())
                        {
                            mapNode.AddChild(subKey, ParseScalar(subValue));
                            ++index;
                        }
                        else
                        {
                            ++index;
                            auto child = ParseNode(lines, index);
                            mapNode.AddChild(subKey, std::move(child));
                        }
                    }

                    listNode.AddChild(std::move(mapNode));
                }
            }

            return listNode;
        }

        // ===== Map 파싱 =====
        auto mapNode = YamlNode::CreateMap();

        while (index < lines.size())
        {
            const auto& [indent, content] = lines[index];

            if (indent != currentIndent)
            {
                break;
            }

            const auto colonPos = content.find(':');
            if (colonPos == std::string::npos)
            {
                ++index;
                continue;
            }

            const std::string key = Trim(content.substr(0, colonPos));
            const std::string value = Trim(content.substr(colonPos + 1));

            if (not value.empty())
            {
                mapNode.AddChild(key, ParseScalar(value));
                ++index;
            }
            else
            {
                ++index;
                auto child = ParseNode(lines, index);
                mapNode.AddChild(key, std::move(child));
            }
        }

        return mapNode;
    }

    YamlNode YamlParser::ParseScalar(const std::string& value)
    {
        return YamlNode::CreateScalar(value);
    }

    std::string YamlParser::Trim(const std::string& str)
    {
        const auto start = str.find_first_not_of(" \t");
        if (start == std::string::npos)
        {
            return "";
        }
        const auto end = str.find_last_not_of(" \t");
        return str.substr(start, end - start + 1);
    }
} // namespace GameLibrary