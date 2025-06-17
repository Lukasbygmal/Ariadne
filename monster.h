#ifndef MONSTER_H
#define MONSTER_H

#include <string>

struct MonsterConfig
{
    std::string name;
    int base_hp;
    int base_strength;
    int base_xp;
};

class Monster
{
private:
    int hp;
    int strength;
    int xp;
    std::string name;

public:
    Monster(const int type, int difficulty);

    int getHP() const;
    int getStrength() const;
    int getXP() const;
    void receiveDamage(int amount);
    std::string to_string() const;
};

#endif