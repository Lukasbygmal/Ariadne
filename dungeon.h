#ifndef DUNGEON_H
#define DUNGEON_H

#include <vector>
#include "room.h"

class Dungeon
{
private:
    std::vector<std::vector<Room>> rooms;
    int size;
    int difficulty;
    int current_x;
    int current_y;
    Room *current_room;

public:
    Dungeon(int size, int difficulty);

    bool goNorth();
    bool goSouth();
    bool goEast();
    bool goWest();

    Room &getCurrentRoom() const;
    Room *returnCurrentRoom() const;
    int getCurrentX() const;
    int getCurrentY() const;
};

#endif