#include "trap.h"

Trap::Trap(int difficulty) : damage(0), avoidance(0), name("")
{
}

int Trap::getDamage() const
{
    return damage;
}

int Trap::getAvoidance() const
{
    return avoidance;
}

std::string Trap::to_string() const
{
    return name;
}

WallArrows::WallArrows(int difficulty) : Trap(difficulty)
{
    name = "Wall Arrows";
    damage = 5; // TODO revisit these numbers
    avoidance = 10;
    adjustForDifficulty(difficulty);
}

void WallArrows::adjustForDifficulty(int difficulty)
{
    damage = damage + 0.3 *difficulty;
}

SwingingAxe::SwingingAxe(int difficulty) : Trap(difficulty)
{
    name = "Swinging Axe";
    damage = 5; // TODO revisit these numbers
    avoidance = 10;
    adjustForDifficulty(difficulty);
}

void SwingingAxe::adjustForDifficulty(int difficulty)
{
    damage = damage + 0.3 *difficulty;
}