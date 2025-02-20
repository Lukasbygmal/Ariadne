#ifndef HANDLE_ACTION_H
#define HANDLE_ACTION_H

#include <iostream>
#include <optional>
#include <string>
#include "dungeon.h"

struct Action
{
    std::string verb;
    std::optional<std::string> direction;
    std::optional<std::string> object;

    Action(const std::string &v,
           std::optional<std::string> d = std::nullopt,
           std::optional<std::string> o = std::nullopt)
        : verb(v), direction(d), object(o) {}

    void print() const
    {
        std::cout << "Verb: " << verb << "\n";
        if (direction)
            std::cout << "Direction: " << *direction << "\n";
        if (object)
            std::cout << "Object: " << *object << "\n";
    }
};

void handle_action(const Action &action, Dungeon &dungeon);

#endif // HANDLE_ACTION_H