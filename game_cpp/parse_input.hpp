#ifndef PARSE_INPUT_H
#define PARSE_INPUT_H

#include <string>
#include "game.hpp"
#include "action.hpp"

// Forward declaration of GameMode enum class
enum class GameMode;

/**
 * Parse user input and convert it to an Action object based on the current game mode
 * 
 * @param input The user's raw input string
 * @param action Reference to an Action object that will be filled with the parsed command
 * @param mode The current GameMode (MENU, DUNGEON, or BATTLE)
 * @return true if input was successfully parsed into a valid action, false otherwise
 */
bool parse_input(const std::string &input, Action &action, GameMode mode);

#endif // PARSE_INPUT_H