#include "dungeon.hpp"
#include <random>
#include <map>
#include <stdexcept>
#include <queue>
#include <set>

std::random_device Dungeon::rd;
std::mt19937 Dungeon::rng(Dungeon::rd());
// DungeonConfig order: size, words, word_length, monster_pool, boss_monster_type, difficulty
const std::map<std::string, DungeonConfig> dungeon_configs = {
    // Bosses:
    {"thal", {4, 4, 3, {1, 2, 3}, 1, 1}},                                // Goblin
    {"vorn", {4, 4, 3, {1, 2, 3}, 2, 4}},                                // Slime
    {"ezra", {5, 4, 3, {1, 2, 3}, 3, 7}},                                // Skeleton
    {"kurn", {4, 5, 3, {1, 2, 3}, 4, 10}},                               // Zombie
    {"zamo", {6, 6, 4, {2, 3, 4, 5}, 5, 13}},                            // Wolf
    {"druv", {5, 6, 4, {2, 3, 4, 5, 6}, 6, 16}},                         // Imp
    {"malq", {5, 7, 4, {3, 4, 5, 6, 7, 8}, 7, 19}},                      // Wraith
    {"xelv", {6, 8, 4, {4, 5, 6, 7, 8, 9, 10}, 8, 22}},                  // Minotaur
    {"ormh", {5, 8, 5, {5, 6, 7, 8, 9, 10}, 9, 25}},                     // Orc
    {"griv", {6, 9, 5, {6, 7, 8, 9, 10, 11}, 10, 28}},                   // Troll
    {"fend", {5, 9, 6, {7, 8, 9, 10, 11}, 11, 31}},                      // Vampire
    {"quar", {7, 9, 5, {8, 9, 10, 11, 12}, 12, 34}},                     // Dragonling
    {"blen", {7, 9, 6, {6, 7, 8, 9, 10, 11}, 6, 37}},                    // Imp reused
    {"xoth", {8, 5, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, 7, 40}}, // Wraith reused
    {"merk", {6, 10, 6, {4, 5, 6, 7, 8, 9, 10, 11}, 12, 43}},            // Dragonling reused
    {"zenk", {7, 10, 7, {1, 2, 3, 4, 5, 6, 7, 8}, 8, 46}}                // Minotaur reused
};

// BFS function to check if a path exists between two points
bool Dungeon::pathExists(int start_x, int start_y, int end_x, int end_y, const std::vector<std::vector<bool>> &blocked) const
{
    if (blocked[start_y][start_x] || blocked[end_y][end_x])
        return false;

    std::queue<std::pair<int, int>> queue;
    std::set<std::pair<int, int>> visited;

    queue.push({start_x, start_y});
    visited.insert({start_x, start_y});

    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    while (!queue.empty())
    {
        auto [x, y] = queue.front();
        queue.pop();

        if (x == end_x && y == end_y)
            return true;

        for (int i = 0; i < 4; i++)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && nx < size && ny >= 0 && ny < size &&
                !blocked[ny][nx] && visited.find({nx, ny}) == visited.end())
            {
                queue.push({nx, ny});
                visited.insert({nx, ny});
            }
        }
    }
    return false;
}

Dungeon::Dungeon(std::string dungeon_name)
{
    name = dungeon_name;

    auto it = dungeon_configs.find(dungeon_name);
    if (it == dungeon_configs.end())
    {
        throw std::invalid_argument("Invalid dungeon name: " + dungeon_name);
    }

    DungeonConfig config = it->second;
    size = config.size;
    words = config.words;
    word_length = config.word_length;
    difficulty = config.difficulty;

    std::uniform_int_distribution<int> roomDist(0, size - 1);
    std::uniform_int_distribution<int> treasureRoomDist(1, size - 2);
    std::uniform_int_distribution<int> chestDist(1, 3);
    std::uniform_int_distribution<int> optionalDist(0, 2);
    std::uniform_int_distribution<int> monsterDist(0, config.monster_pool.size() - 1);
    std::uniform_int_distribution<int> trapDist(1, 3);

    int start_x = roomDist(rng);
    int start_y = size - 1;

    int boss_x = roomDist(rng);
    int boss_y = 0;

    int treasure_x = roomDist(rng);
    int treasure_y = treasureRoomDist(rng);

    // Create blocked rooms while ensuring connectivity
    std::vector<std::vector<bool>> blocked(size, std::vector<bool>(size, false));

    // Block some rooms randomly
    int total_rooms = size * size;
    int rooms_to_block = total_rooms * 0.25; // Magic number for blocking rooms, might want to clean up later. Put in dungeon Config??
    std::uniform_int_distribution<int> blockDist(0, size - 1);

    for (int i = 0; i < rooms_to_block; i++)
    {
        int block_x, block_y;
        bool valid_block = false;
        int attempts = 0;

        while (!valid_block && attempts < 50) // This is also weird magic number that im not sure what to do about
        {
            block_x = blockDist(rng);
            block_y = blockDist(rng);

            // Don't block start, boss, or treasure rooms
            if ((block_x == start_x && block_y == start_y) ||
                (block_x == boss_x && block_y == boss_y) ||
                (block_x == treasure_x && block_y == treasure_y) ||
                blocked[block_y][block_x])
            {
                attempts++;
                continue;
            }

            // Temporarily block this room and check if paths still exist
            blocked[block_y][block_x] = true;

            bool start_to_boss = pathExists(start_x, start_y, boss_x, boss_y, blocked);
            bool start_to_treasure = pathExists(start_x, start_y, treasure_x, treasure_y, blocked);

            if (start_to_boss && start_to_treasure)
            {
                valid_block = true;
            }
            else
            {
                blocked[block_y][block_x] = false;
            }
            attempts++;
        }
    }

    rooms.resize(size);
    for (int y = 0; y < size; ++y)
    {
        rooms[y].reserve(size);
        for (int x = 0; x < size; ++x)
        {
            int chest_type = 0;
            int trap_type = 0;
            int monster_type = 0;
            RoomType room_type = RoomType::NORMAL;

            if (blocked[y][x])
            {
                room_type = RoomType::BLOCKED;
            }
            else if (y == start_y && x == start_x)
            {
                room_type = RoomType::NORMAL;
            }
            else if (y == boss_y && x == boss_x)
            {
                room_type = RoomType::BOSS;
                chest_type = 3;
                monster_type = config.boss_monster_type;
            }
            else if (y == treasure_y && x == treasure_x)
            {
                room_type = RoomType::TREASURE;
                chest_type = 4;
                trap_type = trapDist(rng);
                monster_type = (optionalDist(rng) == 1) ? config.monster_pool[monsterDist(rng)] : 0;
            }
            else
            {
                room_type = RoomType::NORMAL;
                chest_type = (optionalDist(rng) == 0) ? chestDist(rng) : 0;
                trap_type = (optionalDist(rng) == 0) ? trapDist(rng) : 0;
                monster_type = (optionalDist(rng) == 0) ? config.monster_pool[monsterDist(rng)] : 0;
            }

            rooms[y].emplace_back(difficulty, chest_type, trap_type, monster_type, room_type);
        }
    }

    current_x = start_x;
    current_y = start_y;
    current_room = &rooms[current_y][current_x];

    boss_room_x = boss_x;
    boss_room_y = boss_y;
}

bool Dungeon::move(int dx, int dy)
{
    int new_x = current_x + dx;
    int new_y = current_y + dy;

    if (new_x >= 0 && new_x < size && new_y >= 0 && new_y < size)
    {
        // Check if room is blocked
        if (rooms[new_y][new_x].getRoomType() == RoomType::BLOCKED)
        {
            return false;
        }

        current_x = new_x;
        current_y = new_y;
        current_room = &rooms[current_y][current_x];
        return true;
    }
    return false;
}

bool Dungeon::goNorth()
{
    return move(0, -1);
}

bool Dungeon::goSouth()
{
    return move(0, 1);
}

bool Dungeon::goEast()
{
    return move(1, 0);
}

bool Dungeon::goWest()
{
    return move(-1, 0);
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

int Dungeon::getDifficulty() const
{
    return difficulty;
}

std::string Dungeon::capitalizeFirstLetter(const std::string &input) const
{
    if (input.empty())
        return input;

    std::string result = input;
    result[0] = std::toupper(static_cast<unsigned char>(result[0]));
    return result;
}

std::vector<std::vector<int>> Dungeon::dungeonMap() const
{
    std::vector<std::vector<int>> map(size, std::vector<int>(size));
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            switch (rooms[i][j].getRoomType())
            {
            case RoomType::NORMAL:
                map[i][j] = 0;
                break;
            case RoomType::BOSS:
                map[i][j] = 1;
                break;
            case RoomType::TREASURE:
                map[i][j] = 2;
                break;
            case RoomType::BLOCKED:
                map[i][j] = 3;
                break;
            }
        }
    }
    map[getCurrentY()][getCurrentX()] = 4; // player position
    return map;
}

std::string Dungeon::to_string() const
{
    std::string dungeonInfo;
    dungeonInfo = capitalizeFirstLetter(name) + "\n";
    return dungeonInfo;
}