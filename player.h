#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player
{
private:
    std::string name;
    int lvl;
    int xp;
    int xp_to_max;
    int max_hp;
    int current_hp;
    int strength;
    int intelligence;
    int agility;
    int armor;

    void updateXpToMax();
    void checkLevelUp();
    void levelUpBenefits();
    int calculateActualDamage(int incoming_damage) const;

public:
    Player(const std::string &player_name);

    int getLevel() const;
    int getXP() const;
    int getXPToMax() const;
    int getMaxHP() const;
    int getCurrentHP() const;
    int getStrength() const;
    int getIntelligence() const;
    int getAgility() const;
    int getArmor() const;
    std::string getName() const;

    void receiveXP(int amount);
    int receiveDamage(int damage);
    void heal(int amount);
    bool isAlive() const;
    void increaseHP(int amount);
    void increaseStrength(int amount);
    void increaseIntelligence(int amount);
    void increaseAgility(int amount);
    void increaseArmor(int amount);
};

#endif