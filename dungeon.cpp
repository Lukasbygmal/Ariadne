#include "dungeon.h"
#include <random>

Dungeon::Dungeon(int s, int d) : size(s),
                                 difficulty(d),
                                 current_x(0),
                                 current_y(0)
{

    rooms = std::vector<std::vector<Room>>(s, std::vector<Room>(s, Room(d)));

    // Have to do this to get random rooms
    for (int i = 0; i < s; i++)
    {
        for (int j = 0; j < s; j++)
        {
            rooms[i][j] = Room(d);
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