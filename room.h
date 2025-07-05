#ifndef ROOM_H
#define ROOM_H
#include <optional>
#include <string>
#include <memory>
#include "monster.h"
#include "trap.h"
#include "chest.h"

enum class RoomType
{
    NORMAL,
    BOSS,
    TREASURE,
    BLOCKED
};

class Room
{
private:
    std::unique_ptr<Chest> chest;
    std::optional<int> engraving;
    std::unique_ptr<Trap> trap;
    std::unique_ptr<Monster> monster;
    RoomType room_type;

public:
    Room(int difficulty, int chest_type, int engraving, int trap_type, int monster_type, RoomType type);
    Monster *getMonster() const;
    int killMonster();
    int openChest();
    void removeTrap();
    Trap *getTrap() const;
    Chest *getChest() const;
    std::string to_string() const;
    RoomType getRoomType() const;
};
#endif