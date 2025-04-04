#ifndef DUNGEON_H
#define DUNGEON_H

#include <random>
#include <vector>
#include "room.h"

class Dungeon
{
private:
    static std::mt19937 rng;
    static std::random_device rd;
    std::vector<std::vector<Room>> rooms;
    int size;
    int difficulty;
    int current_x;
    int current_y;
    Room *current_room;
    int boss_room_x; // not sure i need this
    int boss_room_y; // not sure i need this

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
    bool isBossRoom() const; // not sure i need this
};

#endif