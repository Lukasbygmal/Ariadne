#ifndef ROOM_H
#define ROOM_H
#include <optional>
#include <string>
#include <memory>
#include "monster.h"
#include "trap.h"
#include "chest.h"
class Room
{
private:
    std::unique_ptr<Chest> chest;
    std::optional<int> engraving;
    std::unique_ptr<Trap> trap;
    std::unique_ptr<Monster> monster;
    int room_number; // -1 = boss room, -2 = treasure room

public:
    Room(int difficulty, int chest_type, int engraving, int trap_type, int monster_type, int room_number);
    Monster *getMonster() const;
    int killMonster();
    int openChest();
    void removeTrap();
    Trap *getTrap() const;
    Chest *getChest() const;
    std::string to_string() const;
};
#endif