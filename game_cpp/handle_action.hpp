/**
 * @file handle_action.hpp
 * @brief Action execution system for game commands
 */
#ifndef HANDLE_ACTION_H
#define HANDLE_ACTION_H

#include "action.hpp"
#include "dungeon.hpp"
#include "game.hpp"

/**
 * @brief Execute a parsed action based on current game state
 *
 * This function serves as the main action dispatcher, routing parsed user commands
 * to appropriate game systems based on the current GameMode. It handles:
 *
 * **Menu Mode:**
 * - "enter [dungeon] [difficulty]" - Enter specified dungeon
 * - "buy [amount] [stat]" - Purchase stat improvements (hp/strength/armor/agility)
 *
 * **Dungeon Mode:**
 * - "go [direction]" - Move through dungeon (north/south/east/west)
 * - "open chest" - Open treasure chests
 * - "exit" - Leave dungeon and return to menu
 *
 * **Battle Mode:**
 * - Any input is treated as battle typing input
 *
 * @param action Parsed action structure containing verb, direction, and object
 * @param game Reference to game instance for state modification
 * @return true if action was successfully executed, false if invalid/failed
 *
 * @note Buy amounts are limited to 1-999
 * @note Movement automatically triggers room hazard checks
 * @note Battle input bypasses normal command parsing
 */

bool handle_action(const Action &action, Game &game);

#endif