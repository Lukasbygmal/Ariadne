#ifndef PARSE_INPUT_H
#define PARSE_INPUT_H

#include <string>
#include "action.h"

enum class GameMode
{
    DUNGEON,
    MENU,
    BATTLE
};

bool parse_input(const std::string &input, Action &action, GameMode mode = GameMode::DUNGEON);

#endif