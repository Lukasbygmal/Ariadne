/**
 * @file dungeon.h
 * @brief Dungeon generation and handling
 */
#ifndef DUNGEON_H
#define DUNGEON_H

#include <random>
#include <vector>
#include "room.h"

/**
 * @brief Configuration parameters for dungeon generation
 */
struct DungeonConfig
{
    int size;
    int words;       // needs to be >4, since defend words are -2
    int word_length; // needs to be >2, since defend words are -1
    std::vector<int> monster_pool;
    int boss_monster_type;
    int base_difficulty;
};

/**
 * @brief Manages dungeon layout, room generation, and player navigation
 *
 * The Dungeon class generates a procedural grid-based dungeon with special rooms
 * (boss, treasure), blocked passages, and ensures connectivity between key locations.
 * Provides movement mechanics and dungeon state information.
 */
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
    /**
     * @brief Constructs a new dungeon with specified configuration
     * @param dungeon_name Name identifier for the dungeon configuration
     * @param string_difficulty Difficulty level ("easy", "medium", "hard")
     * @throws std::invalid_argument if dungeon_name is not found in configurations
     */
    Dungeon(std::string dungeon_name, std::string string_difficulty);

    /**
     * @brief Attempts to move player [direction], north: -y, south: +y, east: +x, west: -x
     * @return true if movement was successful, false if blocked or out of bounds
     */
    bool goNorth();
    bool goSouth();
    bool goEast();
    bool goWest();

    /**
     * @brief Gets reference to the room at player's current position
     * @return Reference to the current Room object
     */
    Room &getCurrentRoom() const;

    /**
     * @brief Generates a 2D integer map representing the dungeon layout
     * @return 2D vector where values represent room types (0=normal, 1=boss, 2=treasure, 3=blocked, 4=player)
     */
    std::vector<std::vector<int>> dungeonMap() const;

    /**
     * @brief Gets player's current X coordinate
     * @return Current X position in the dungeon grid
     */
    int getCurrentX() const;

    /**
     * @brief Gets player's current Y coordinate
     * @return Current Y position in the dungeon grid
     */
    int getCurrentY() const;

    /**
     * @brief Gets the calculated difficulty level for this dungeon
     * @return Total difficulty (base difficulty + difficulty modifier)
     */
    int getDifficulty() const;

    /**
     * @brief Gets the word length used for word-based game mechanics
     * @return Length of words used in this dungeon
     */
    int getWordLength() const { return word_length; }

    /**
     * @brief Gets the number of words available for word-based mechanics
     * @return Number of words configured for this dungeon
     */
    int getWords() const { return words; }

    /**
     * @brief Generates a string representation of the dungeon
     * @return Formatted string with dungeon name and difficulty
     */
    std::string to_string() const;
};

#endif