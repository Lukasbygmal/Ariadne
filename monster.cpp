#include "monster.h"

Monster::Monster(int difficulty) : hp(0), strength(0), base_xp(0), name("")
{
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
    return base_xp; // should eventually change this
}

void Monster::receiveDamage(int amount)
{
    hp = hp - amount;
}

std::string Monster::to_string() const
{
    return name;
}

Skeleton::Skeleton(int difficulty) : Monster(difficulty)
{
    name = "Skeleton";
    hp = 5; // temporarily made really easy for testing
    strength = 3;
    base_xp = 100;
    adjustForDifficulty(difficulty);
}

void Skeleton::adjustForDifficulty(int difficulty)
{
    hp *= difficulty;
    strength *= (difficulty * 0.5);
    base_xp *= difficulty;
}

Goblin::Goblin(int difficulty) : Monster(difficulty)
{
    name = "Goblin";
    hp = 3; // temporarily made really easy for testing
    strength = 2;
    base_xp = 75;
    adjustForDifficulty(difficulty);
}

void Goblin::adjustForDifficulty(int difficulty)
{
    hp *= (difficulty * 0.8);
    strength *= (difficulty * 0.6);
    base_xp *= difficulty;
}

Minotaur::Minotaur(int difficulty) : Monster(difficulty)
{
    name = "Minotaur";
    hp = 1; // temporarily made really easy for testing
    strength = 3;
    base_xp = 500;
    adjustForDifficulty(difficulty);
}

void Minotaur::adjustForDifficulty(int difficulty)
{
    hp *= (difficulty * 1.2);
    strength *= difficulty;
    base_xp *= (difficulty * 1.5);
}

Wolf::Wolf(int difficulty) : Monster(difficulty)
{
    name = "Wolf";
    hp = 2; // temporarily made really easy for testing
    strength = 5;
    base_xp = 90;
    adjustForDifficulty(difficulty);
}

void Wolf::adjustForDifficulty(int difficulty)
{
    hp *= (difficulty * 0.5);
    strength *= (difficulty * 0.5);
    base_xp *= difficulty;
}