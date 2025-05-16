#ifndef PLAYER_H
#define PLAYER_H

#include <string>

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
    int intelligence;
    int agility;
    int armor;

    void updateXpToMax();
    void checkLevelUp();
    void levelUpBenefits();
    int calculateActualDamage(int incoming_damage) const;

public:
    Player(const std::string &player_name, int init_lvl, int init_xp, int init_gold, int init_max_hp, int init_strength, int init_agility, int init_armor);

    int getLevel() const;
    int getGold() const;
    int getXP() const;
    int getXPToMax() const;
    int getMaxHP() const;
    int getCurrentHP() const;
    int getStrength() const;
    int getIntelligence() const;
    int getAgility() const;
    int getArmor() const;
    std::string getName() const;

    void healToMax();
    void receiveXP(int amount);
    void receiveGold(int amount);
    void decreaseGold(int amount);
    int receiveDamage(int damage);
    int receiveDamageParry(int damage);
    void heal(int amount);
    bool isAlive() const;
    void increaseHP(int amount);
    void increaseStrength(int amount);
    void increaseIntelligence(int amount);
    void increaseAgility(int amount);
    void increaseArmor(int amount);
    void dead();
};

#endif