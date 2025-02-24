#ifndef ROOM_H
#define ROOM_H

#include <random>
#include <optional>
#include <string>

class Room
{
private:
    static std::mt19937 rng;
    static std::random_device rd;

    int type;
    int path_n;
    int path_e;
    int path_s;
    int path_w;
    std::optional<int> chest;
    std::optional<int> corpse;
    std::optional<int> engraving;
    std::optional<int> trap;
    std::optional<int> monster;

public:
    Room(int difficulty);
    std::optional<int> getMonster() const;
    std::optional<int> getTrap() const;

    std::string to_string() const;
};

#endif
