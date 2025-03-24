#include "room.h"
#include <iostream>

Room::Room(int difficulty, int _path_n, int _path_e, int _path_s, int _path_w, int chest_type, int _corpse, int _engraving, int trap_type, int monster_type)
{
    path_n = _path_n;
    path_e = _path_e;
    path_s = _path_s;
    path_w = _path_w;
    corpse = _corpse;
    engraving = _engraving;

    switch (chest_type)
    {
    case 0:
        break;
    case 1:
        chest = new SmallChest(difficulty);
        break;
    case 2:
        chest = new MediumChest(difficulty);
        break;
    case 3:
        chest = new LargeChest(difficulty);
        break;
    case 4:
        chest = new Artifact(difficulty);
        break;
    }

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

int Room::killMonster()
{
    int xp = monster.value()->getXP();
    monster.reset();
    return xp;
}

int Room::openChest()
{  
    std::cout << "I get into open chest\n";
    int gold = chest.value()->getGold();
    chest.reset();
    return gold;
}

void Room::removeTrap()
{
    trap.reset();
}

std::optional<Trap *> Room::getTrap() const
{
    return trap;
}

std::optional<Chest *> Room::getChest() const
{
    return chest;
}

std::string Room::to_string() const
{
    std::string roomInfo = "Room:\n\n";
    roomInfo += "Paths: " + std::to_string(path_s) + ", " +
                std::to_string(path_n) + ", " +
                std::to_string(path_e) + ", " +
                std::to_string(path_w) + "\n\n";
    if (chest){
        roomInfo += "Chest: " + (*chest)->to_string() + "\n\n";
    }
    if (corpse)
        roomInfo += "Corpse: " + std::to_string(*corpse) + "\n\n";
    if (engraving)
        roomInfo += "Engraving: " + std::to_string(*engraving) + "\n\n";
    return roomInfo;
}
