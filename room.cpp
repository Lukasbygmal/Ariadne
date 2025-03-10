#include "room.h"
#include <iostream>

Room::Room(int difficulty, int _path_n, int _path_e, int _path_s, int _path_w, int _chest, int _corpse, int _engraving, int trap_type, int monster_type)
{
    path_n = _path_n;
    path_e = _path_e;
    path_s = _path_s;
    path_w = _path_w;
    chest = _chest;
    corpse = _corpse;
    engraving = _engraving;

    switch (trap_type)
    {
    case 0:
        break;
    case 1:
        trap = new WallArrows(difficulty);
        break;
    case 2:
        trap = new SwingingAxe(difficulty);
        break;
    }
    switch (monster_type)
    {
    case 0:
        break;
    case 1:
        monster = new Skeleton(difficulty);
        break;
    case 2:
        monster = new Goblin(difficulty);
        break;
    case 3:
        monster = new Minotaur(difficulty);
        break;
    case 4:
        monster = new Wolf(difficulty);
        break;
    }
}

std::optional<Monster *> Room::getMonster() const
{
    return monster;
}

void Room::killMonster()
{
    // should return xp eventually
    monster.reset();
}

void Room::removeTrap()
{
    trap.reset();
}

std::optional<Trap *> Room::getTrap() const
{
    return trap;
}

std::string Room::to_string() const
{
    std::string roomInfo = "Room:\n\n";
    roomInfo += "Paths: " + std::to_string(path_s) + ", " +
                std::to_string(path_n) + ", " +
                std::to_string(path_e) + ", " +
                std::to_string(path_w) + "\n\n";
    if (chest)
        roomInfo += "Chest: " + std::to_string(*chest) + "\n\n";
    if (corpse)
        roomInfo += "Corpse: " + std::to_string(*corpse) + "\n\n";
    if (engraving)
        roomInfo += "Engraving: " + std::to_string(*engraving) + "\n\n";
    return roomInfo;
}
