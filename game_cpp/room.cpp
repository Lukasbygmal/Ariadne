#include "room.hpp"
Room::Room(int difficulty, int chest_type, int trap_type, int monster_type, RoomType room_type)
{
    this->room_type = room_type;
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
    case 3:
        trap = std::make_unique<RollingBoulder>(difficulty);
        break;
    }
    if (monster_type != 0)
    {
        bool is_boss = false;
        if (room_type == RoomType::BOSS)
        {
            is_boss = true;
        }
        monster = std::make_unique<Monster>(monster_type, difficulty, is_boss);
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

RoomType Room::getRoomType() const
{
    return room_type;
}

std::string Room::to_string() const
{
    std::string roomInfo;
    if (room_type == RoomType::BOSS)
    {
        roomInfo = "Boss Room:\n";
    }
    else if (room_type == RoomType::TREASURE)
    {
        roomInfo = "Treasure Room:\n";
    }
    else
    {
        roomInfo = "Room:\n";
    }

    if (chest)
    {
        roomInfo += chest->to_string() + "\n";
    }
    return roomInfo;
}