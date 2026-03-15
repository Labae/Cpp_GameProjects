//
// Created by Labae on 3/15/26.
//

#ifndef JOBCONFIG_HPP
#define JOBCONFIG_HPP
#include "SkillConfig.hpp"

#include <string>
#include <vector>

namespace TextRPG
{
    struct JobConfig
    {
        std::string name{};
        int32_t baseHp{};
        int32_t baseMp{};
        int32_t baseAttack{};
        int32_t baseDefense{};
        int32_t hpPerLevel{};
        int32_t mpPerLevel{};
        int32_t attackPerLevel{};
        int32_t defensePerLevel{};
        std::vector<SkillConfig> skills{};
    };
} // namespace TextRPG

#endif // JOBCONFIG_HPP
