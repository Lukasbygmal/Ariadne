/**
 * @file monster.hpp
 * @brief Monster handling
 */
#ifndef MONSTER_H
#define MONSTER_H

#include <string>

/**
 * @brief Base configuration data for monster types
 */
struct MonsterConfig
{
    std::string name;
    int base_hp;
    int base_strength;
    int base_xp;
};

/**
 * @brief Represents a combat monster with scaled stats based on difficulty
 *
 * Monsters are created from predefined configurations and have their stats
 * scaled based on dungeon difficulty. Boss monsters receive additional
 * stat bonuses and modified display names.
 */
class Monster
{
private:
    int hp;
    int strength;
    int xp;
    std::string name;
    bool is_boss;

public:
    /**
     * @brief Constructs a monster with difficulty-scaled stats
     * @param type Monster type ID corresponding to predefined configurations
     * @param difficulty Scaling factor for monster stats (applied quadratically)
     * @param is_boss Whether this monster is a boss (receives 1.2x difficulty multiplier)
     * @throws std::invalid_argument if monster type is not found in configurations
     */
    Monster(const int type, int difficulty, bool is_boss);

    /**
     * @brief Gets the monster's current health points
     * @return Current HP value
     */
    int getHP() const;

    /**
     * @brief Gets the monster's attack strength
     * @return Strength value used for damage calculations
     */
    int getStrength() const;

    /**
     * @brief Gets experience points awarded for defeating this monster
     * @return XP value based on difficulty-scaled base XP
     */
    int getXP() const;

    /**
     * @brief Reduces monster's health by specified damage amount
     * @param amount Damage to subtract from current HP
     * @note HP can go negative; caller should check for death condition
     */
    void receiveDamage(int amount);

    /**
     * @brief Generates display name for the monster
     * @return Monster name with "Boss" suffix if is_boss is true
     */
    std::string to_string() const;
};

#endif