//
// Created by Labae on 3/15/26.
//

#ifndef PLAYERDATA_HPP
#define PLAYERDATA_HPP
#include "SkillConfig.hpp"

#include <string>
#include <vector>

namespace TextRPG
{
    struct PlayerData
    {
        std::string jobName{};
        int32_t level{1};
        int32_t exp{};
        int32_t expToNext{30};

        int32_t hp{};
        int32_t maxHp{};
        int32_t mp{};
        int32_t maxMp{};
        int32_t attack{};
        int32_t defense{};

        std::vector<SkillConfig> skills{};
        int32_t gold{};
    };
} // namespace TextRPG

#endif // PLAYERDATA_HPP
