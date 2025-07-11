/**
 * @file chest.h
 * @brief Treasure chest system with quadratic gold scaling based on difficulty
 */

#ifndef CHEST_H
#define CHEST_H

#include <string>

/**
 * @brief Base class for all treasure chests with difficulty-based gold rewards
 */
class Chest
{
protected:
    int difficulty;
    int base_gold;
    std::string name;

public:
    /**
     * @brief Constructs a chest with specified difficulty level
     * @param difficulty The difficulty level affecting gold rewards
     */
    Chest(int difficulty);
    virtual ~Chest() = default;

    /**
     * @brief Calculates and returns the gold value of the chest
     * @return The gold amount using quadratic scaling: base_gold * difficulty²
     */
    int getGold();

    /**
     * @brief Gets the string representation of the chest
     * @return The name of the chest type
     */
    virtual std::string to_string() const;
};

class SmallChest : public Chest
{
public:
    SmallChest(int difficulty);
};

class MediumChest : public Chest
{
public:
    MediumChest(int difficulty);
};

class LargeChest : public Chest
{
public:
    LargeChest(int difficulty);
};

class Artifact : public Chest
{
public:
    Artifact(int difficulty);
};

#endif