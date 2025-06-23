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
    int agility;
    int armor;

    void updateXpToMax();
    bool checkLevelUp();
    void levelUpBenefits();

public:
    Player(const std::string &player_name, int init_lvl, int init_xp, int init_gold, int init_max_hp, int init_strength, int init_agility, int init_armor);

    int getLevel() const;
    int getGold() const;
    int getXP() const;
    int getXPToMax() const;
    int getMaxHP() const;
    int getCurrentHP() const;
    int getStrength() const;
    int getAgility() const;
    int getArmor() const;
    std::string getName() const;

    void healToMax();
    bool receiveXP(int amount);
    void receiveGold(int amount);
    void decreaseGold(int amount);
    int damage();
    int receiveDamage(int damage);
    int receiveDamageParry(int damage);
    void heal(int amount);
    bool isAlive() const;
    void increaseHP(int amount);
    void increaseStrength(int amount);
    void increaseAgility(int amount);
    void increaseArmor(int amount);
    void dead();
};

#endif