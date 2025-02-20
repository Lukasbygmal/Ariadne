#ifndef PARSE_INPUT_H
#define PARSE_INPUT_H

#include <string>
#include <optional>

enum class GameMode
{
    DUNGEON,
    MENU,
    BATTLE
};

class Action
{
public:
    Action(const std::string &verb,
           const std::optional<std::string> &direction = std::nullopt,
           const std::optional<std::string> &object = std::nullopt)
        : verb_(verb), direction_(direction), object_(object) {}

    std::string verb_;
    std::optional<std::string> direction_;
    std::optional<std::string> object_;
};

bool parse_input(const std::string &input, Action &action, GameMode mode = GameMode::DUNGEON);

#endif // PARSE_INPUT_H