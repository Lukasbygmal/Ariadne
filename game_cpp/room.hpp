/**
 * @file room.hpp
 * @brief Room system
 */

#ifndef ROOM_H
#define ROOM_H
#include <string>
#include <memory>
#include "monster.hpp"
#include "trap.hpp"
#include "chest.hpp"

/**
 * @brief Defines the functional type of a room within the dungeon
 */
enum class RoomType
{
    NORMAL,
    BOSS,
    TREASURE,
    BLOCKED
};

/**
 * @brief Represents a single room in the dungeon with interactive elements
 *
 * Manages room contents including monsters, traps, and treasure chests.
 * Provides methods to interact with room elements and handles their
 * lifecycle through smart pointers.
 */
class Room
{
private:
    std::unique_ptr<Chest> chest;
    std::unique_ptr<Trap> trap;
    std::unique_ptr<Monster> monster;
    RoomType room_type;

public:
    /**
     * @brief Constructs a room with specified contents based on type codes
     * @param difficulty Base difficulty level for scaling room contents
     * @param chest_type Chest type (0=none, 1=small, 2=medium, 3=large, 4=artifact)
     * @param trap_type Trap type (0=none, 1=wall arrows, 2=swinging axe, 3=rolling boulder)
     * @param monster_type Monster type ID (0=none, 1-12=specific monsters)
     * @param type Room category determining special behaviors
     */
    Room(int difficulty, int chest_type, int trap_type, int monster_type, RoomType type);

    /**
     * @brief Gets pointer to the monster in this room
     * @return Pointer to Monster object, or nullptr if no monster present
     */
    Monster *getMonster() const;

    /**
     * @brief Defeats the room's monster and returns experience reward
     * @return Experience points gained from killing the monster
     * @note Removes the monster from the room permanently
     */
    int killMonster();

    /**
     * @brief Opens the room's chest and returns gold reward
     * @return Gold amount obtained from the chest
     * @note Removes the chest from the room permanently
     */
    int openChest();

    /**
     * @brief Disables and removes the room's trap
     * @note Removes the trap from the room permanently
     */
    void removeTrap();

    /**
     * @brief Gets pointer to the trap in this room
     * @return Pointer to Trap object, or nullptr if no trap present
     */
    Trap *getTrap() const;

    /**
     * @brief Gets pointer to the chest in this room
     * @return Pointer to Chest object, or nullptr if no chest present
     */
    Chest *getChest() const;

    /**
     * @brief Generates a string description of the room and its contents
     * @return Formatted string with room type and chest information
     */
    std::string to_string() const;

    /**
     * @brief Gets the functional type of this room
     * @return RoomType enum value indicating room category
     */
    RoomType getRoomType() const;
};
#endif