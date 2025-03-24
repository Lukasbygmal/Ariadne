#ifndef MONSTER_H
#define MONSTER_H

#include <string>

class Monster
{
protected:
    int hp;
    int strength;
    int base_xp;
    std::string name;

public:
    Monster(int difficulty);
    virtual ~Monster() = default;

    int getHP() const;
    int getStrength() const;
    int getXP() const;
    void receiveDamage(int amount);
    virtual std::string to_string() const;

    virtual void adjustForDifficulty(int difficulty) = 0;
};

class Skeleton : public Monster
{
public:
    Skeleton(int difficulty);
    void adjustForDifficulty(int difficulty) override;
};

class Goblin : public Monster
{
public:
    Goblin(int difficulty);
    void adjustForDifficulty(int difficulty) override;
};

class Minotaur : public Monster
{
public:
    Minotaur(int difficulty);
    void adjustForDifficulty(int difficulty) override;
};

class Wolf : public Monster
{
public:
    Wolf(int difficulty);
    void adjustForDifficulty(int difficulty) override;
};

#endif