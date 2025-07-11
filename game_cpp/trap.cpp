#include "trap.h"

Trap::Trap(int difficulty) : damage(0), name("")
{
}

int Trap::getDamage() const
{
    return damage;
}

std::string Trap::to_string() const
{
    return name;
}

WallArrows::WallArrows(int difficulty) : Trap(difficulty)
{
    name = "Wall Arrows";
    damage = 5; 
    damage = damage *difficulty;
}

SwingingAxe::SwingingAxe(int difficulty) : Trap(difficulty)
{
    name = "Swinging Axe";
    damage = 5;
    damage = damage *difficulty;
}

RollingBoulder::RollingBoulder(int difficulty) : Trap(difficulty)
{
    name = "RollingBoulder";
    damage = 10;
    damage = damage *difficulty;
}
