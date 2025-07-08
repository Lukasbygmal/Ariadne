#ifndef DUNGEON_H
#define DUNGEON_H

#include <random>
#include <vector>
#include "room.h"

struct DungeonConfig
{
    int size;
    int words;       // needs to be >4, since defend words are -2
    int word_length; // needs to be >2, since defend words are -1
    std::vector<int> monster_pool;
    int boss_monster_type;
    int base_difficulty;
};

class Dungeon
{
private:
    static std::mt19937 rng;
    static std::random_device rd;
    std::vector<std::vector<Room>> rooms;
    std::string name;
    std::string difficulty_string;
    int size;
    int words;       // needs to be >4, since defend words are -2
    int word_length; // needs to be >2, since defend words are -1
    int difficulty;
    int current_x;
    int current_y;
    Room *current_room;
    int boss_room_x;
    int boss_room_y;
    bool move(int dx, int dy);
    std::string capitalizeFirstLetter(const std::string &input) const;
    bool pathExists(int start_x, int start_y, int end_x, int end_y, const std::vector<std::vector<bool>> &blocked) const;

public:
    Dungeon(std::string dungeon_name, std::string string_difficulty);

    bool goNorth();
    bool goSouth();
    bool goEast();
    bool goWest();

    Room &getCurrentRoom() const;
    std::vector<std::vector<int>> dungeonMap() const;
    int getCurrentX() const;
    int getCurrentY() const;
    int getDifficulty() const;
    bool isBossRoom() const; // not sure i need this
    int getWordLength() const { return word_length; }
    int getWords() const { return words; }
    std::string to_string() const;
};

#endif