#include "dungeon.h"
#include <random>
#include <map>

std::random_device Dungeon::rd;
std::mt19937 Dungeon::rng(Dungeon::rd());

std::map<std::string, DungeonConfig> dungeon_configs = {
    {"Thalgrin", {4, {1, 2}, 1, 1}},
    {"Vornak", {5, {1, 2, 3}, 2, 4}},
    {"Ezrath", {5, {2, 3}, 2, 7}},
    {"Kundrel", {6, {2, 3, 4}, 3, 10}},
    {"Zamorran", {6, {3, 4}, 3, 13}},
    {"Druvok", {7, {2, 3, 4}, 4, 16}},
    {"Malquor", {7, {1, 3, 4}, 4, 19}},
    {"Xelveth", {8, {2, 4}, 4, 22}},
    {"Ormathal", {8, {1, 2, 4}, 4, 25}},
    {"Grivnox", {10, {1, 3, 4}, 4, 28}}};
Dungeon::Dungeon(std::string dungeon_name, std::string string_difficulty) : size(4),
                                                                            difficulty(1),
                                                                            current_x(0),
                                                                            current_y(0),
                                                                            boss_room_x(0),
                                                                            boss_room_y(0)
{
    int difficulty_level = 1;
    if (string_difficulty == "medium")
        difficulty_level = 2;
    else if (string_difficulty == "hard")
        difficulty_level = 3;

    auto it = dungeon_configs.find(dungeon_name);

    DungeonConfig config = it->second;
    size = config.size;
    int base_difficulty = config.base_difficulty;
    int int_difficulty = difficulty_level + base_difficulty;

    std::uniform_int_distribution<int> roomDist(0, size - 1);
    std::uniform_int_distribution<int> treasureRoomDist(1, size - 2);
    std::uniform_int_distribution<int> pathDist(0, 3);
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
            int path_n = pathDist(rng);
            int path_e = pathDist(rng);
            int path_s = pathDist(rng);
            int path_w = pathDist(rng);
            int chest_type = 0;
            int corpse = 0;
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
            rooms[i].emplace_back(int_difficulty, path_n, path_e, path_s, path_w, chest_type, corpse, engraving, trap_type, monster_type);
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