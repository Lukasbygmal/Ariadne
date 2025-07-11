/**
 * @file player.h
 * @brief Player character system with stats, leveling, and combat mechanics
 */
#ifndef PLAYER_H
#define PLAYER_H

#include <string>

/**
 * @brief Represents a player character with RPG progression and combat systems
 *
 * Manages player stats, experience/leveling, gold, health, and combat
 * calculations. Provides automatic leveling with stat bonuses and damage mitigation
 * through armor mechanics.
 */
class Player
{
private:
    std::string name;
    int lvl;
    int xp;
    int gold;
    int xp_to_max;
    int max_hp;
    int current_hp;
    int strength;
    int agility;
    int armor;

    void updateXpToMax();
    bool checkLevelUp();
    void levelUpBenefits();

public:
    /**
     * @brief Constructs a player with initial stats and calculates XP requirements
     * @param player_name Display name for the player
     * @param init_lvl Starting level
     * @param init_xp Starting experience points
     * @param init_gold Starting gold amount
     * @param init_max_hp Starting maximum health points
     * @param init_strength Starting strength stat
     * @param init_agility Starting agility stat
     * @param init_armor Starting armor value
     */
    Player(const std::string &player_name, int init_lvl, int init_xp, int init_gold, int init_max_hp, int init_strength, int init_agility, int init_armor);

    /**
     * @brief Gets the player's current level
     * @return Current level
     * @note Lvl is capped at 100
     */
    int getLevel() const;

    /**
     * @brief Gets the player's [thing]
     * @return [#thing]
     */
    int getGold() const;
    int getXP() const;
    int getXPToMax() const;
    int getMaxHP() const;
    int getCurrentHP() const;
    int getStrength() const;
    int getAgility() const;
    int getArmor() const;
    std::string getName() const; // Not actually used yet

    /**
     * @brief Restores player health to maximum
     */
    void healToMax();

    /**
     * @brief Adds experience points and handles automatic leveling
     * @param amount XP to add
     * @return true if player leveled up, false otherwise
     */
    bool receiveXP(int amount);

    /**
     * @brief Adds gold to player's inventory
     * @param amount Gold to add
     */
    void receiveGold(int amount);

    /**
     * @brief Removes gold from player's inventory
     * @param amount Gold to subtract
     * @note Can result in negative gold balance
     */
    void decreaseGold(int amount);

    /**
     * @brief Calculates player's attack damage output
     * @return Damage value based on level, strength, and agility
     */
    int damage();

    /**
     * @brief Applies damage to player with armor mitigation
     * @param damage Raw damage amount before armor reduction
     * @return Actual damage taken after armor calculations
     */
    int receiveDamage(int damage);

    /**
     * @brief Applies reduced damage when player successfully parries
     * @param damage Raw damage amount before parry reduction
     * @return Actual damage taken after 10% parry reduction and armor
     */
    int receiveDamageParry(int damage);

    /**
     * @brief Checks if player is alive
     * @return true if current HP > 0, false otherwise
     */
    bool isAlive() const;

    /**
     * @brief Permanently increases [stat]
     * @param amount [of stat to add]
     */
    void increaseHP(int amount);
    void increaseStrength(int amount);
    void increaseAgility(int amount);
    void increaseArmor(int amount);

    /**
     * @brief Applies death penalties to player
     * @note Reduces XP by 10% and gold by 50%
     */
    void dead();
};

#endif