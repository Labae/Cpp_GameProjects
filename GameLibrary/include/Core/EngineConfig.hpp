#pragma once

#include <stdint.h>
#include <string>

namespace GameLibrary
{
    struct EngineConfig
    {
        int32_t screenWidth{800};
        int32_t screenHeight{600};
        std::string title{"Game"};
        std::string fontPath{};
        std::string configPath{};
    };
} // namespace GameLibrary