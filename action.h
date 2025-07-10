#ifndef ACTION_H
#define ACTION_H

#include <string>
#include <optional>

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