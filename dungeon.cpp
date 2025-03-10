#include "dungeon.h"
#include <random>

std::random_device Dungeon::rd;
std::mt19937 Dungeon::rng(Dungeon::rd());

Dungeon::Dungeon(int s, int d) : size(s),
                                 difficulty(d),
                                 current_x(0),
                                 current_y(0)
{
    rng.seed(rd());

    std::uniform_int_distribution<int> pathDist(0, 3);
    std::uniform_int_distribution<int> chestDist(0, 1);
    std::uniform_int_distribution<int> optionalDist(0, 2);
    std::uniform_int_distribution<int> monsterDist(1, 4);
    std::uniform_int_distribution<int> trapDist(0, 1);

    rooms.resize(s);
    for (int i = 0; i < s; ++i)
    {
        rooms[i].reserve(s);
        for (int j = 0; j < s; ++j)
        {
            int path_n = pathDist(rng);
            int path_e = pathDist(rng);
            int path_s = pathDist(rng);
            int path_w = pathDist(rng);
            int chest = 0;     // have to change this
            int corpse = 0;    // and this
            int engraving = 0; // and this since they are 0 forever now
            int trap_type = (optionalDist(rng) == 1) ? trapDist(rng) : 0;
            int monster_type = (optionalDist(rng) == 1) ? monsterDist(rng) : 0;

            rooms[i].emplace_back(difficulty, path_n, path_e, path_s, path_w, chest, corpse, engraving, trap_type, monster_type);
        }
    }

    current_room = &rooms[current_y][current_x];
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