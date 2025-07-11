#include "chest.hpp"

Chest::Chest(int difficulty) : difficulty(difficulty), base_gold(0), name("")
{
}

int Chest::getGold()
{
    return base_gold * this->difficulty * this->difficulty;
}

std::string Chest::to_string() const
{
    return name;
}

SmallChest::SmallChest(int difficulty) : Chest(difficulty)
{
    name = "Small Chest";
    base_gold = 1;
}

MediumChest::MediumChest(int difficulty) : Chest(difficulty)
{
    name = "Medium Chest";
    base_gold = 2;
}

LargeChest::LargeChest(int difficulty) : Chest(difficulty)
{
    name = "Large Chest";
    base_gold = 4;
}

Artifact::Artifact(int difficulty) : Chest(difficulty)
{
    name = "Artifact";
    base_gold = 10;
}