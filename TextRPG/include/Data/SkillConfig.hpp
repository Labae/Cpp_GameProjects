//
// Created by Labae on 3/15/26.
//

#ifndef SKILLCONFIG_HPP
#define SKILLCONFIG_HPP
#include <string>

namespace TextRPG
{
    struct SkillConfig
    {
        std::string name{};
        std::string description{};
        int32_t mpCost{};
        int32_t power{};
    };
}

#endif //SKILLCONFIG_HPP
