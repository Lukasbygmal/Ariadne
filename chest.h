#ifndef CHEST_H
#define CHEST_H

#include <string>

class Chest
{
protected:
    int difficulty;
    int base_gold;
    std::string name;

public:
    Chest(int difficulty);
    virtual ~Chest() = default;

    int getGold();
    virtual std::string to_string() const;

};

class SmallChest : public Chest
{
public:
    SmallChest(int difficulty);
};

class MediumChest : public Chest
{
public:
    MediumChest(int difficulty);
};

class LargeChest : public Chest
{
public:
    LargeChest(int difficulty);
};

class Artifact : public Chest
{
public:
    Artifact(int difficulty);
};

#endif