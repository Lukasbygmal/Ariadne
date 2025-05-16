#include "player.h"
#include <algorithm>

Player::Player(const std::string &player_name, int init_lvl, int init_xp, int init_gold, int init_max_hp, int init_strength, int init_agility, int init_armor) : name(player_name),
                                                                                                                                                                 lvl(init_lvl),
                                                                                                                                                                 gold(init_gold),
                                                                                                                                                                 xp(init_xp),
                                                                                                                                                                 max_hp(init_max_hp),
                                                                                                                                                                 current_hp(init_max_hp),
                                                                                                                                                                 strength(init_strength),
                                                                                                                                                                 intelligence(12), // most likely want to remove, but not sure yet
                                                                                                                                                                 agility(init_agility),
                                                                                                                                                                 armor(init_armor)
{
    updateXpToMax();
}

int Player::getLevel() const { return lvl; }
int Player::getGold() const { return gold; }
int Player::getXP() const { return xp; }
int Player::getXPToMax() const { return xp_to_max; }
int Player::getMaxHP() const { return max_hp; }
int Player::getCurrentHP() const { return current_hp; }
int Player::getStrength() const { return strength; }
int Player::getIntelligence() const { return intelligence; }
int Player::getAgility() const { return agility; }
int Player::getArmor() const { return armor; }
std::string Player::getName() const { return name; }

void Player::dead()
{
    xp = 0;
    gold = gold / 2;
    current_hp = 100;
}

void Player::receiveXP(int amount)
{
    xp += amount;
    checkLevelUp();
}

void Player::receiveGold(int amount)
{
    gold += amount;
}

void Player::decreaseGold(int amount)
{
    gold -= amount;
}

int Player::receiveDamage(int damage)
{
    int actual_damage = calculateActualDamage(damage);
    current_hp = std::max(0, current_hp - actual_damage);
    return actual_damage;
}

int Player::receiveDamageParry(int damage)
{
    int damage_after_parry = receiveDamage(damage) * 0.9; // want to change how this works eventually, need to ponder how to use stats (maybe agility used here?)
    return damage_after_parry;
}

void Player::heal(int amount)
{
    current_hp = std::min(max_hp, current_hp + amount);
}

void Player::healToMax()
{
    current_hp = max_hp;
}

bool Player::isAlive() const
{
    return current_hp > 0;
}

void Player::increaseHP(int amount)
{
    max_hp += amount;
}

void Player::increaseStrength(int amount)
{
    strength += amount;
}

void Player::increaseIntelligence(int amount)
{
    intelligence += amount;
}

void Player::increaseAgility(int amount)
{
    agility += amount;
}

void Player::increaseArmor(int amount)
{
    armor += amount;
}

void Player::updateXpToMax()
{
    xp_to_max = (lvl + 1) * (lvl + 100) * 5;
}

void Player::checkLevelUp()
{
    while (xp >= xp_to_max)
    {
        xp -= xp_to_max;
        lvl++;
        updateXpToMax();
        levelUpBenefits();
    }
}

void Player::levelUpBenefits()
{
    // don't know how/what, need to know combat first
}

int Player::calculateActualDamage(int incoming_damage) const
{
    float damage_reduction = armor / (armor + 100.0f);
    return static_cast<int>(incoming_damage * (1.0f - damage_reduction));
}