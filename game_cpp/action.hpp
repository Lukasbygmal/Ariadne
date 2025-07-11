/**
 * @file action.hpp
 * @brief Action structure for representing parsed game commands
 */
#ifndef ACTION_H
#define ACTION_H

#include <string>
#include <optional>

/**
 * @brief Represents a parsed user command with verb and optional parameters
 *
 * The Action struct encapsulates a user command that has been parsed from raw input.
 * It uses a flexible design with optional parameters to handle different command
 * formats across various game modes (menu, dungeon, battle).
 *
 * The struct follows a verb-object-modifier pattern where:
 * - verb: The primary action to perform (required)
 * - direction: Optional directional parameter (e.g., "north", "south")
 * - object: Optional target object or secondary parameter
 * @note The flexible parameter interpretation depends on the current game mode context
 * @see parse_input() for how different game modes interpret the optional parameters
 */
struct Action
{
    std::string verb;
    std::optional<std::string> direction;
    std::optional<std::string> object;

    Action(const std::string &v,
           std::optional<std::string> d = std::nullopt,
           std::optional<std::string> o = std::nullopt)
        : verb(v), direction(d), object(o) {}
};

#endif