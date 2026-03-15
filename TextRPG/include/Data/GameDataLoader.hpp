//
// Created by Labae on 3/15/26.
//

#ifndef GAMEDATALOADER_HPP
#define GAMEDATALOADER_HPP
#include "JobConfig.hpp"

#include <vector>

namespace TextRPG
{
    class GameDataLoader final
    {
    public:
        [[nodiscard]] static std::vector<JobConfig> LoadJobs(const std::string& path);
    };
} // namespace TextRPG

#endif // GAMEDATALOADER_HPP
