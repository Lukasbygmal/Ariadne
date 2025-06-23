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
    std::optional<Chest *> chest;
    std::optional<int> engraving;
    std::optional<Trap *> trap;
    std::optional<Monster *> monster;

public:
    Room(int difficulty, int chest_type, int engraving, int trap_type, int monster_type);
    std::optional<Monster *> getMonster() const;
    int killMonster();
    int openChest();
    void removeTrap();
    std::optional<Trap *> getTrap() const;
    std::optional<Chest *> getChest() const;

    std::string to_string() const;
};

#endif
