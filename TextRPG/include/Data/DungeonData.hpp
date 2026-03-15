//
// Created by Labae on 3/15/26.
//

#ifndef DUNGEONDATA_HPP
#define DUNGEONDATA_HPP
#include <cstdint>

namespace TextRPG
{
    enum class ERoom : uint8_t
    {
        Combat,
        Treasure,
        Rest,
        Event,
        Boss
    };

    struct DungeonData
    {
        int32_t currentFloor{1};
        int32_t currentRoom{};
        int32_t roomsPerFloor{4};
        int32_t totalFloors{3};
        ERoom currentRoomType{};
        bool atBranch{};
    };
}

#endif //DUNGEONDATA_HPP
