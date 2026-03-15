//
// Created by Labae on 3/15/26.
//

#ifndef GAMESESSION_HPP
#define GAMESESSION_HPP

#include "DungeonData.hpp"
#include "PlayerData.hpp"

namespace TextRPG
{
    struct GameSessionData
    {
        PlayerData player{};
        DungeonData dungeon{};
    };
} // namespace TextRPG

#endif // GAMESESSION_HPP
