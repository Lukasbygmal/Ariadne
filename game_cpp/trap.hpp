/**
 * @file trap.h
 * @brief Trap system for dungeon encounters with difficulty scaling
 */
#ifndef TRAP_H
#define TRAP_H

#include <string>

/**
 * @brief Base class for all dungeon traps with difficulty-based damage scaling
 */
class Trap
{ // Would be a good idea to use config like monster and dungeon if i add more trap types
protected:
    int damage;
    std::string name;

public:
    /**
     * @brief Constructs a trap with specified difficulty
     * @param difficulty The difficulty level affecting trap damage
     */
    Trap(int difficulty);
    virtual ~Trap() = default;

    /**
     * @brief Gets the damage value of the trap
     * @return The damage points this trap inflicts
     */
    int getDamage() const;

    /**
     * @brief Gets the string representation of the trap
     * @return The name of the trap
     */
    virtual std::string to_string() const;
};

class WallArrows : public Trap
{
public:
    WallArrows(int difficulty);
};

class SwingingAxe : public Trap
{
public:
    SwingingAxe(int difficulty);
};

class RollingBoulder : public Trap
{
public:
    RollingBoulder(int difficulty);
};
#endif