#include "monster.h"
#include <map>
#include <stdexcept>

// MonsterConfig order: name, base_hp, base_strength, base_xp
const std::map<int, MonsterConfig> monster_configs = {
    {1, {"Goblin", 5, 1, 50}},
    {2, {"Slime", 5, 2, 60}},
    {3, {"Skeleton", 5, 3, 70}},
    {4, {"Zombie", 5, 4, 90}},
    {5, {"Wolf", 5, 5, 80}},
    {6, {"Imp", 5, 6, 70}},
    {7, {"Wraith", 5, 9, 140}},
    {8, {"Minotaur", 5, 10, 150}},
    {9, {"Orc", 5, 12, 130}},
    {10, {"Troll", 5, 15, 180}},
    {11, {"Vampire", 5, 18, 200}},
    {12, {"Dragonling", 5, 20, 195}}};

Monster::Monster(const int type, int difficulty, bool is_boss)
{
    this->is_boss = is_boss;
    if (is_boss)
    {
        difficulty = difficulty * 2;
    }
    auto it = monster_configs.find(type);
    if (it == monster_configs.end())
    {
        throw std::invalid_argument("Invalid monster type: " + type);
    }

    const MonsterConfig &config = it->second;

    name = config.name;
    hp = static_cast<int>(config.base_hp * difficulty); // This needs balancing
    strength = static_cast<int>(config.base_strength * difficulty);
    xp = static_cast<int>(config.base_xp * difficulty);
}

int Monster::getHP() const
{
    return hp;
}

int Monster::getStrength() const
{
    return strength;
}

int Monster::getXP() const
{
    return xp;
}

void Monster::receiveDamage(int amount)
{
    hp = hp - amount;
}

std::string Monster::to_string() const
{
    if (is_boss)
    {
        return (name + " Boss");
    }
    return name;
}