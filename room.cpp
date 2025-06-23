#include "room.h"
#include <iostream>

Room::Room(int difficulty, int chest_type, int _engraving, int trap_type, int monster_type)
{
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
    if (monster_type != 0)
    {
        monster = new Monster(monster_type, difficulty);
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
    std::string roomInfo = "In the room:\n\n";
    if (chest)
    {
        roomInfo += (*chest)->to_string() + "\n\n";
    }
    if (engraving)
        roomInfo += "Engraving \n\n";
    return roomInfo;
}
