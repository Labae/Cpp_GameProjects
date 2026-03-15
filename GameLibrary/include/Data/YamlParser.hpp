//
// Created by Labae on 3/15/26.
//

#ifndef YAMLPARSER_HPP
#define YAMLPARSER_HPP
#include "YamlNode.hpp"

namespace GameLibrary
{
    class YamlParser final
    {
    public:
        [[nodiscard]] static YamlNode LoadFromFile(const std::string& path);
        [[nodiscard]] static YamlNode LoadFromString(const std::string& content);

    private:
        struct Line
        {
            int32_t indent{};
            std::string content{};
        };

        [[nodiscard]] static std::vector<Line> Tokenize(const std::string& content);
        [[nodiscard]] static std::string Trim(const std::string& str);

        [[nodiscard]] static YamlNode ParseNode(const std::vector<Line>& lines, size_t& index);
        [[nodiscard]] static YamlNode ParseScalar(const std::string& value);
    };
} // namespace GameLibrary

#endif // YAMLPARSER_HPP
