#include "dungeon.h"
#include <random>
#include <map>
#include <stdexcept>

std::random_device Dungeon::rd;
std::mt19937 Dungeon::rng(Dungeon::rd());
// DungeonConfig order: size, monster_pool, boss_monster_type, base_difficulty
const std::map<std::string, DungeonConfig> dungeon_configs = {
    // Bosses:
    {"thal", {4, {1, 2, 3}, 1, 1}},                                 // Goblin
    {"vorn", {5, {1, 2, 3}, 2, 4}},                                 // Slime
    {"ezra", {5, {1, 2, 3}, 3, 7}},                                 // Skeleton
    {"kurn", {6, {1, 2, 3}, 4, 10}},                                // Zombie
    {"zamo", {6, {2, 3, 4, 5}, 5, 13}},                             // Wolf
    {"druv", {7, {2, 3, 4, 5, 6}, 6, 16}},                          // Imp
    {"malq", {7, {3, 4, 5, 6, 7, 8}, 7, 19}},                       // Wraith
    {"xelv", {8, {4, 5, 6, 7, 8, 9, 10}, 8, 22}},                   // Minotaur
    {"ormh", {8, {5, 6, 7, 8, 9, 10}, 9, 25}},                      // Orc
    {"griv", {10, {6, 7, 8, 9, 10, 11}, 10, 28}},                   // Troll
    {"fend", {11, {7, 8, 9, 10, 11}, 11, 31}},                      // Vampire
    {"quar", {12, {8, 9, 10, 11, 12}, 12, 34}},                     // Dragonling
    {"blen", {13, {6, 7, 8, 9, 10, 11}, 6, 37}},                    // Imp reused
    {"xoth", {20, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, 7, 40}}, // Wraith reused
    {"merk", {15, {4, 5, 6, 7, 8, 9, 10, 11}, 12, 43}},             // Dragonling reused
    {"zenk", {16, {1, 2, 3, 4, 5, 6, 7, 8}, 3, 46}}                 // Skeleton reused
};

Dungeon::Dungeon(std::string dungeon_name, std::string string_difficulty)
{
    name = dungeon_name;
    difficulty_string = string_difficulty;
    int difficulty_level = 1;
    if (string_difficulty == "medium")
        difficulty_level = 2;
    else if (string_difficulty == "hard")
        difficulty_level = 3;

    auto it = dungeon_configs.find(dungeon_name);
    if (it == dungeon_configs.end())
    {
        throw std::invalid_argument("Invalid dungeon name: " + dungeon_name);
    }

    DungeonConfig config = it->second;
    size = config.size;
    int base_difficulty = config.base_difficulty;
    int int_difficulty = difficulty_level + base_difficulty;

    std::uniform_int_distribution<int> roomDist(0, size - 1);
    std::uniform_int_distribution<int> treasureRoomDist(1, size - 2);
    std::uniform_int_distribution<int> chestDist(0, 1);
    std::uniform_int_distribution<int> optionalDist(0, 2);
    std::uniform_int_distribution<int> monsterDist(0, config.monster_pool.size() - 1);
    std::uniform_int_distribution<int> trapDist(0, 1);

    int start_x = roomDist(rng);
    int start_y = 0;

    int boss_x = roomDist(rng);
    int boss_y = size - 1;

    int treasure_x = roomDist(rng);
    int treasure_y = treasureRoomDist(rng);

    rooms.resize(size);
    for (int i = 0; i < size; ++i)
    {
        rooms[i].reserve(size);
        for (int j = 0; j < size; ++j)
        {
            int chest_type = 0;
            int engraving = 0;
            int trap_type = 0;
            int monster_type = 0;
            if (i == start_y && j == start_x) // start room
            {
            }
            else if (i == boss_y && j == boss_x) // boss room
            {
                monster_type = config.boss_monster_type;
            }
            else if (i == treasure_y && j == treasure_x) // treasure room
            {
                chest_type = 4;
                trap_type = trapDist(rng);
                monster_type = (optionalDist(rng) == 1) ? config.monster_pool[monsterDist(rng)] : 0;
            }
            else // normal room
            {
                chest_type = chestDist(rng);
                trap_type = (optionalDist(rng) == 1) ? trapDist(rng) : 0;
                monster_type = (optionalDist(rng) == 1) ? config.monster_pool[monsterDist(rng)] : 0;
            }
            rooms[i].emplace_back(int_difficulty, chest_type, engraving, trap_type, monster_type);
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

std::string Dungeon::to_string() const
{
    std::string dungeonInfo;
    dungeonInfo = name + " - " + difficulty_string + "\n\n";
    return dungeonInfo;
}