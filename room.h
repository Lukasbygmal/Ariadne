#ifndef ROOM_H
#define ROOM_H

#include <optional>
#include <string>
#include "monster.h"
#include "trap.h"
#include "chest.h"

class Room
{
private:
    int path_n;
    int path_e;
    int path_s;
    int path_w;
    std::optional<Chest *> chest;
    std::optional<int> corpse;
    std::optional<int> engraving;
    std::optional<Trap *> trap;
    std::optional<Monster *> monster;

public:
    Room(int difficulty, int path_n, int path_e, int path_s, int path_w, int chest_type, int corpse, int engraving, int trap_type, int monster_type);
    std::optional<Monster *> getMonster() const;
    int killMonster();
    int openChest();
    void removeTrap();
    std::optional<Trap *> getTrap() const;
    std::optional<Chest *> getChest() const;

    std::string to_string() const;
};

#endif
