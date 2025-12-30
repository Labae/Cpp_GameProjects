#pragma once

#include <cstdint>

namespace SnakeGame
{
    struct GameData
    {
        int32_t lastScore{};
        int32_t highScore{};
        float bgmVolume{1.0f};
        float sfxVolume{1.0f};
    };
} // namespace SnakeGame