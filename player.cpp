#include "player.h"
#include <algorithm>
#include <cmath>

Player::Player(const std::string &player_name, int init_lvl, int init_xp, int init_gold, int init_max_hp, int init_strength, int init_agility, int init_armor) : name(player_name),
                                                                                                                                                                 lvl(init_lvl),
                                                                                                                                                                 gold(init_gold),
                                                                                                                                                                 xp(init_xp),
                                                                                                                                                                 max_hp(init_max_hp),
                                                                                                                                                                 current_hp(init_max_hp),
                                                                                                                                                                 strength(init_strength),
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
int Player::getAgility() const { return agility; }
int Player::getArmor() const { return armor; }
std::string Player::getName() const { return name; }

void Player::dead()
{
    xp = 0;
    gold = gold / 2;
}

bool Player::receiveXP(int amount)
{
    xp += amount;
    bool did_level = checkLevelUp();
    return did_level;
}

void Player::receiveGold(int amount)
{
    gold += amount;
}

void Player::decreaseGold(int amount)
{
    gold -= amount;
}

int Player::damage()
{
    int damage = (strength + agility * 0.2); // balancing, and should agility be used here at all?
    return damage;
}

int Player::receiveDamage(int damage)
{
    float scaling_constant = 75.0f; // balancing?
    float percent_multiplier = scaling_constant / (scaling_constant + armor);
    float flat_factor = 0.2f; // balancing?
    int flat_reduction = static_cast<int>(armor * flat_factor);

    int reduced_damage = static_cast<int>(damage * percent_multiplier) - flat_reduction;
    int actual_damage = std::max(1, reduced_damage);
    current_hp = std::max(0, current_hp - actual_damage); // to not have negative hp
    return actual_damage;
}

int Player::receiveDamageParry(int damage)
{
    int damage_after_parry = static_cast<int>(damage * 0.9); // balancing?
    receiveDamage(damage_after_parry);
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
    xp_to_max = (lvl + 1) * (lvl + 100); // temporarily decreased, was xp_to_max = (lvl + 1) * (lvl + 100) * 5; before
}

bool Player::checkLevelUp()
{
    if (xp >= xp_to_max)
    {
        while (xp >= xp_to_max) // have to make it this ugly to cover corner case of leveling more than 1 level
        {
            xp -= xp_to_max;
            lvl++;
            updateXpToMax();
            levelUpBenefits();
        }
        return true;
    }

    return false;
}

void Player::levelUpBenefits()
{
    int scaling = std::ceil(lvl / 5.0);
    increaseHP(25 * scaling);
    increaseStrength(scaling);
    increaseAgility(scaling);
    increaseArmor(scaling);
}