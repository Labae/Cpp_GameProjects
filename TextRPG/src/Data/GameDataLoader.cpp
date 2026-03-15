//
// Created by Labae on 3/15/26.
//

#include "Data/GameDataLoader.hpp"

#include "Data/YamlParser.hpp"
#include "Systems/Logger.hpp"

namespace TextRPG
{

    std::vector<JobConfig> GameDataLoader::LoadJobs(const std::string& path)
    {
        const auto root = GameLibrary::YamlParser::LoadFromFile(path);
        std::vector<JobConfig> jobs;

        for (const auto& node : root["jobs"].AsList())
        {
            JobConfig job;
            job.name = node["name"].AsString();
            job.baseHp = node["baseHp"].AsInt();
            job.baseMp = node["baseMp"].AsInt();
            job.baseAttack = node["baseAttack"].AsInt();
            job.baseDefense = node["baseDefense"].AsInt();
            job.hpPerLevel = node["hpPerLevel"].AsInt();
            job.mpPerLevel = node["mpPerLevel"].AsInt();
            job.attackPerLevel = node["attackPerLevel"].AsInt();
            job.defensePerLevel = node["defensePerLevel"].AsInt();

            for (const auto& skillNode : node["skills"].AsList())
            {
                SkillConfig skill;
                skill.name = skillNode["name"].AsString();
                skill.description = skillNode["description"].AsString();
                skill.mpCost = node["mpCost"].AsInt();
                skill.power = node["power"].AsInt();
                job.skills.emplace_back(std::move(skill));
            }

            jobs.emplace_back(std::move(job));
        }

        GameLibrary::Logger::Info("Loaded " + std::to_string(jobs.size()) + " jobs from " + path);
        return jobs;
    }
} // namespace TextRPG