#include "monster.h"
#include <map>
#include <stdexcept>

// MonsterConfig order: name, base_hp, base_strength, base_xp
const std::map<int, MonsterConfig> monster_configs = {
    {1, {"Skeleton", 5, 3, 100}},
    {2, {"Goblin", 3, 2, 75}},
    {3, {"Minotaur", 1, 3, 500}},
    {4, {"Wolf", 2, 5, 90}}};

Monster::Monster(const int type, int difficulty)
{
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
    return name;
}