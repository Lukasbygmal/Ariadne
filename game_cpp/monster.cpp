#include "monster.hpp"
#include <map>
#include <stdexcept>

// MonsterConfig order: name, base_hp, base_strength, base_xp
const std::map<int, MonsterConfig> monster_configs = {
    {1, {"Hydra", 25, 1, 60}},
    {2, {"Cyclops", 50, 1, 80}},
    {3, {"Cerberus", 25, 3, 100}},
    {4, {"Chimera", 200, 4, 120}},
    {5, {"Scylla", 100, 5, 80}},
    {6, {"Harpy", 100, 6, 70}},
    {7, {"Satyr", 250, 9, 140}},
    {8, {"Gorgon", 700, 10, 150}},
    {9, {"Empusa", 400, 12, 130}},
    {10, {"Lamia", 500, 15, 180}},
    {11, {"Echidna", 450, 18, 200}},
    {12, {"Minotaur", 550, 20, 195}}};

Monster::Monster(const int type, int difficulty, bool is_boss)
{
    this->is_boss = is_boss;
    if (is_boss)
    {
        difficulty = difficulty * 1.2;
    }
    auto it = monster_configs.find(type);
    if (it == monster_configs.end())
    {
        throw std::invalid_argument("Invalid monster type: " + type);
    }

    const MonsterConfig &config = it->second;

    name = config.name;
    hp = static_cast<int>(config.base_hp * difficulty * difficulty);
    strength = static_cast<int>(config.base_strength * difficulty * difficulty);
    xp = static_cast<int>(config.base_xp * difficulty * difficulty);
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