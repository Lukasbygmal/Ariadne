#ifndef ACTION_H
#define ACTION_H

#include <string>
#include <optional>
#include <iostream>

struct Action {
    std::string verb;
    std::optional<std::string> direction;
    std::optional<std::string> object;

    Action(const std::string& v,
           std::optional<std::string> d = std::nullopt,
           std::optional<std::string> o = std::nullopt)
        : verb(v), direction(d), object(o) {}

    void print() const {
        std::cout << "Verb: " << verb << "\n";
        if (direction)
            std::cout << "Direction: " << *direction << "\n";
        if (object)
            std::cout << "Object: " << *object << "\n";
    }
};

#endif