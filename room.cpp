#include "room.h"
#include <iostream>
Room::Room(int difficulty, int chest_type, int engraving, int trap_type, int monster_type)
{
    this->engraving = engraving;
    switch (chest_type)
    {
    case 0:
        break;
    case 1:
        chest = std::make_unique<SmallChest>(difficulty);
        break;
    case 2:
        chest = std::make_unique<MediumChest>(difficulty);
        break;
    case 3:
        chest = std::make_unique<LargeChest>(difficulty);
        break;
    case 4:
        chest = std::make_unique<Artifact>(difficulty);
        break;
    }

    switch (trap_type)
    {
    case 0:
        break;
    case 1:
        trap = std::make_unique<WallArrows>(difficulty);
        break;
    case 2:
        trap = std::make_unique<SwingingAxe>(difficulty);
        break;
    }
    if (monster_type != 0)
    {
        monster = std::make_unique<Monster>(monster_type, difficulty);
    }
}

Monster *Room::getMonster() const
{
    return monster.get();
}

int Room::killMonster()
{
    int xp = monster->getXP();
    monster.reset();
    return xp;
}

int Room::openChest()
{
    std::cout << "I get into open chest\n";
    int gold = chest->getGold();
    chest.reset();
    return gold;
}
void Room::removeTrap()
{
    trap.reset();
}

Trap *Room::getTrap() const
{
    return trap.get();
}

Chest *Room::getChest() const
{
    return chest.get();
}

std::string Room::to_string() const
{
    std::string roomInfo = "In the room:\n";
    if (chest)
    {
        roomInfo += chest->to_string() + "\n";
    }
    if (engraving)
        roomInfo += "Engraving \n";
    return roomInfo;
}