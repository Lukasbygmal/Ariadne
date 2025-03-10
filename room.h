#ifndef ROOM_H
#define ROOM_H

#include <optional>
#include <string>
#include "monster.h"
#include "trap.h"

class Room
{
private:
    int path_n;
    int path_e;
    int path_s;
    int path_w;
    std::optional<int> chest;
    std::optional<int> corpse;
    std::optional<int> engraving;
    std::optional<Trap *> trap;
    std::optional<Monster *> monster;

public:
    Room(int difficulty, int path_n, int path_e, int path_s, int path_w, int chest, int corpse, int engraving, int trap_type, int monster_type);
    std::optional<Monster *> getMonster() const;
    void killMonster();
    void removeTrap();
    std::optional<Trap *> getTrap() const;

    std::string to_string() const;
};

#endif
