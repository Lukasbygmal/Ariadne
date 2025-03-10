#include "dungeon.h"
#include <random>

std::random_device Dungeon::rd;
std::mt19937 Dungeon::rng(Dungeon::rd());

Dungeon::Dungeon(int s, int d) : size(s),
                                 difficulty(d),
                                 current_x(0),
                                 current_y(0),
                                 boss_room_x(0),
                                 boss_room_y(0)
{
    rng.seed(rd());

    std::uniform_int_distribution<int> roomDist(0, size - 1);
    std::uniform_int_distribution<int> treasureRoomDist(1, size - 2);
    std::uniform_int_distribution<int> pathDist(0, 3);
    std::uniform_int_distribution<int> chestDist(0, 1);
    std::uniform_int_distribution<int> optionalDist(0, 2);
    std::uniform_int_distribution<int> monsterDist(1, 4);
    std::uniform_int_distribution<int> trapDist(0, 1);

    int path_n, path_e, path_s, path_w, chest, corpse, engraving, trap_type, monster_type;

    int start_x = roomDist(rng);
    int start_y = 0;

    int boss_x = roomDist(rng);
    int boss_y = size - 1;

    int treasure_x = roomDist(rng);
    int treasure_y = treasureRoomDist(rng);

    rooms.resize(s);
    for (int i = 0; i < s; ++i)
    {
        rooms[i].reserve(s);
        for (int j = 0; j < s; ++j)
        {
            if (i == start_y && j == start_x) // start room
            {
                path_n = pathDist(rng);
                path_e = pathDist(rng);
                path_s = pathDist(rng);
                path_w = pathDist(rng);
                chest = 0;
                corpse = 0;
                engraving = 0;
                trap_type = 0;
                monster_type = 0;
                rooms[i].emplace_back(difficulty, path_n, path_e, path_s, path_w, chest, corpse, engraving, trap_type, monster_type);
            }
            else if (i == boss_y && j == boss_x) // boss room, will be end room too
            {
                path_n = pathDist(rng);
                path_e = pathDist(rng);
                path_s = pathDist(rng);
                path_w = pathDist(rng);
                chest = 0;
                corpse = 0;
                engraving = 0;
                trap_type = 0;
                monster_type = 4;
                rooms[i].emplace_back(difficulty, path_n, path_e, path_s, path_w, chest, corpse, engraving, trap_type, monster_type);
            }
            else if (i == treasure_y && j == treasure_x) // Treasure room
            {
                path_n = pathDist(rng);
                path_e = pathDist(rng);
                path_s = pathDist(rng);
                path_w = pathDist(rng);
                chest = 2;
                corpse = 0;
                engraving = 0;
                trap_type = trapDist(rng);
                monster_type = (optionalDist(rng) == 1) ? monsterDist(rng) : 0;
                rooms[i].emplace_back(difficulty, path_n, path_e, path_s, path_w, chest, corpse, engraving, trap_type, monster_type);
            }
            else // normal room
            {
                path_n = pathDist(rng);
                path_e = pathDist(rng);
                path_s = pathDist(rng);
                path_w = pathDist(rng);
                chest = 0;     // have to change this
                corpse = 0;    // and this
                engraving = 0; // and this since they are 0 forever now
                trap_type = (optionalDist(rng) == 1) ? trapDist(rng) : 0;
                monster_type = (optionalDist(rng) == 1) ? monsterDist(rng) : 0;
                rooms[i].emplace_back(difficulty, path_n, path_e, path_s, path_w, chest, corpse, engraving, trap_type, monster_type);
            }
        }
    }

    current_x = start_x;
    current_y = start_y;
    current_room = &rooms[current_y][current_x];

    boss_room_x = boss_x;
    boss_room_y = boss_y;
}

bool Dungeon::goNorth()
{
    if (current_y > 0)
    {
        current_y--;
        current_room = &rooms[current_y][current_x];
        return true;
    }
    return false;
}

bool Dungeon::goSouth()
{
    if (current_y < size - 1)
    {
        current_y++;
        current_room = &rooms[current_y][current_x];
        return true;
    }
    return false;
}

bool Dungeon::goEast()
{
    if (current_x < size - 1)
    {
        current_x++;
        current_room = &rooms[current_y][current_x];
        return true;
    }
    return false;
}

bool Dungeon::goWest()
{
    if (current_x > 0)
    {
        current_x--;
        current_room = &rooms[current_y][current_x];
        return true;
    }
    return false;
}

Room &Dungeon::getCurrentRoom() const
{
    return *current_room;
}

Room *Dungeon::returnCurrentRoom() const
{
    return current_room;
}

int Dungeon::getCurrentX() const
{
    return current_x;
}

int Dungeon::getCurrentY() const
{
    return current_y;
}

bool Dungeon::isBossRoom() const
{
    if (current_x == boss_room_x && current_y == boss_room_y)
    {
        return true;
    }
    return false;
}