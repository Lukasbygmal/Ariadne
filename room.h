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
    int path_l;
    int path_m;
    int path_r;
    std::optional<int> chest;
    std::optional<int> corpse;
    std::optional<int> engraving;
    std::optional<int> trap;
    std::optional<int> monster;

public:
    Room(int difficulty);

    std::string to_string() const;
};

#endif // ROOM_H
